// Copyright (C) 2018-2023 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include <string>
#include <vector>

#include "extension_mngr.h"
#include "graph.h"
#include "graph_context.h"
#include "openvino/runtime/icompiled_model.hpp"
#include "openvino/runtime/iinfer_request.hpp"
#include "openvino/runtime/iplugin.hpp"
#include "openvino/runtime/isync_infer_request.hpp"
#include "openvino/runtime/threading/thread_local.hpp"

namespace ov {
namespace intel_cpu {

class CompiledModel : public ov::ICompiledModel {
public:
    typedef std::shared_ptr<CompiledModel> Ptr;

    CompiledModel(const std::shared_ptr<ov::Model>& model,
                  const std::shared_ptr<const ov::IPlugin>& plugin,
                  const Config& cfg,
                  const ExtensionManager::Ptr& extMgr,
                  const bool loaded_from_cache = false);

    std::shared_ptr<ov::IAsyncInferRequest> create_infer_request() const override;

    void export_model(std::ostream& model) const override;

    std::shared_ptr<const ov::Model> get_runtime_model() const override;

    ov::Any get_property(const std::string& name) const override;

    void set_property(const ov::AnyMap& properties) override {
        OPENVINO_ASSERT_HELPER(::ov::NotImplemented,
                               "",
                               false,
                               "Not Implemented",
                               "CompiledModel::set_property is not supported by CPU plugin!");
    };

private:
    std::shared_ptr<ov::ISyncInferRequest> create_sync_infer_request() const override;
    friend class SyncInferRequest;

    const std::shared_ptr<ov::Model> m_model;
    std::vector<std::shared_ptr<ov::IVariableState>> m_memory_states;
    const std::shared_ptr<const ov::IPlugin> m_plugin;
    std::shared_ptr<ov::threading::ITaskExecutor> m_task_executor = nullptr;      //!< Holds a task executor
    std::shared_ptr<ov::threading::ITaskExecutor> m_callback_executor = nullptr;  //!< Holds a callback executor

    // Generic synchronization primitive on CompiledModel level.
    // Usage example: helps to avoid data races during CPU Graph initialization in multi-streams scenario
    std::shared_ptr<std::mutex> m_mutex;
    Config m_cfg;
    ExtensionManager::Ptr extensionManager;
    mutable std::atomic_int m_numRequests = {0};
    std::string m_name;
    struct GraphGuard : public Graph {
        std::mutex _mutex;
        struct Lock : public std::unique_lock<std::mutex> {
            explicit Lock(GraphGuard& graph) : std::unique_lock<std::mutex>(graph._mutex), _graph(graph) {}
            GraphGuard& _graph;
        };
    };

    const bool m_loaded_from_cache;
    // WARNING: Do not use m_graphs directly.
    mutable std::deque<GraphGuard> m_graphs;
    mutable SocketsWeights m_socketWeights;

    /* WARNING: Use get_graph() function to get access to graph in current stream.
     * NOTE: Main thread is interpreted as master thread of external stream so use this function to get access to graphs
     *       even from main thread
     */
    GraphGuard::Lock get_graph() const;

    ov::Any get_metric_legacy(const std::string& name, const GraphGuard& graph) const;
};

}   // namespace intel_cpu
}   // namespace ov


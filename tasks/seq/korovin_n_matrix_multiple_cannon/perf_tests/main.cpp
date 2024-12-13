// Copyright 2023 Nesterov Alexander
#include <gtest/gtest.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "seq/korovin_n_matrix_multiple_cannon/include/ops_seq.hpp"

TEST(korovin_n_matrix_multiple_cannon_seq, test_task_run) {
  int m = 512;
  int n = 512;
  int k = 512;

  std::vector<double> A(m * n, 0.0);
  for (int i = 0; i < m * n; i++) {
    A[i] = i % 100 + 1;
  }

  std::vector<double> B(n * k, 0.0);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < k; j++) {
      B[i * k + j] = (i + j) % 50 + 1;
    }
  }

  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs_count.emplace_back(m);
  taskData->inputs_count.emplace_back(n);
  taskData->inputs_count.emplace_back(k);
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t*>(A.data()));
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t*>(B.data()));

  std::vector<double> C(m * k, 0.0);
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t*>(C.data()));
  taskData->outputs_count.emplace_back(C.size());

  auto testTask = std::make_shared<korovin_n_matrix_multiple_cannon_seq::TestTaskSequential>(taskData);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTask);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}

TEST(korovin_n_matrix_multiple_cannon_seq, test_pipeline_run) {
  int m = 512;
  int n = 512;
  int k = 512;

  std::vector<double> A(m * n, 0.0);
  for (int i = 0; i < m * n; i++) {
    A[i] = i % 100 + 1;
  }

  std::vector<double> B(n * k, 0.0);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < k; j++) {
      B[i * k + j] = (i + j) % 50 + 1;
    }
  }

  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs_count.emplace_back(m);
  taskData->inputs_count.emplace_back(n);
  taskData->inputs_count.emplace_back(k);
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t*>(A.data()));
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t*>(B.data()));

  std::vector<double> C(m * k, 0.0);
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t*>(C.data()));
  taskData->outputs_count.emplace_back(C.size());

  auto testTask = std::make_shared<korovin_n_matrix_multiple_cannon_seq::TestTaskSequential>(taskData);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTask);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}

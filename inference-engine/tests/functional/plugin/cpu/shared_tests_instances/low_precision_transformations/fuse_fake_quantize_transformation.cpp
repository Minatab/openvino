// Copyright (C) 2020 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "low_precision_transformations/fuse_fake_quantize_transformation.hpp"
#include <vector>
#include <gtest/gtest.h>

using namespace LayerTestsDefinitions;
using namespace InferenceEngine::details;

namespace {

const std::vector<FuseFakeQuantizeTransformationTestValues> testValues = {
    // 1) Multiply
    {
        ngraph::Shape{1, 3, 16, 16},
        LayerTestsUtils::LayerTransformationParamsNGraphFactory::createParamsU8I8(),
        {
            ngraph::element::f32,
            { },
            ngraph::element::f32,
            { {}, {}, { 0.01f } },
            ngraph::element::f32,
            { 256ul, {}, { 0.f }, { 2.55f }, { 0.f }, { 2.55f } }
        }
    },
    // 1) Subtract + Multiply
    {
        ngraph::Shape{1, 3, 16, 16},
        LayerTestsUtils::LayerTransformationParamsNGraphFactory::createParamsU8I8(),
        {
            ngraph::element::f32,
            { },
            ngraph::element::f32,
            { {}, { -128 }, { 0.01f } },
            ngraph::element::f32,
            { 256ul, {}, { 0.f }, { 2.55f }, { 0.f }, { 2.55f } }
        }
    },
    // 1) Convert + Subtract + Multiply
    {
        ngraph::Shape{1, 3, 16, 16},
        LayerTestsUtils::LayerTransformationParamsNGraphFactory::createParamsU8I8(),
        {
            ngraph::element::f32,
            { },
            ngraph::element::u8,
            { {ngraph::element::f32}, { -128 }, { 0.01f } },
            ngraph::element::f32,
            { 256ul, {}, { 0.f }, { 2.55f }, { 0.f }, { 2.55f } }
        }
    },
    // 1) Convert + Subtract + Multiply 2) Add
    {
        ngraph::Shape{1, 3, 16, 16},
        LayerTestsUtils::LayerTransformationParamsNGraphFactory::createParamsU8I8(),
        {
            ngraph::element::f32,
            { {128}, ngraph::element::f32 },
            ngraph::element::u8,
            { {ngraph::element::f32}, { -128 }, { 0.01f } },
            ngraph::element::f32,
            { 256ul, {}, { 0.f }, { 2.55f }, { 0.f }, { 2.55f } }
        }
    },
};

INSTANTIATE_TEST_CASE_P(smoke_LPT, FuseFakeQuantizeTransformation,
    ::testing::Combine(
        ::testing::Values(CommonTestUtils::DEVICE_CPU),
        ::testing::ValuesIn(testValues)),
    FuseFakeQuantizeTransformation::getTestCaseName);
}  // namespace

// Tencent is pleased to support the open source community by making ncnn available.
//
// Copyright (C) 2017 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// https://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "elu.h"

namespace ncnn {

ELU::ELU()
{
    one_blob_only = true;
    support_inplace = true;
}

int ELU::load_param(const ParamDict& pd)
{
    alpha = pd.get(0, 0.1f);

    return 0;
}

int ELU::forward_inplace(Mat& bottom_top_blob, const Option& opt) const
{
    int w = bottom_top_blob.w;
    int h = bottom_top_blob.h;
    int d = bottom_top_blob.d;
    int channels = bottom_top_blob.c;
    int size = w * h * d;

    #pragma omp parallel for num_threads(opt.num_threads)
    for (int q = 0; q < channels; q++)
    {
        float* ptr = bottom_top_blob.channel(q);

        for (int i = 0; i < size; i++)
        {
            if (ptr[i] < 0.f)
                ptr[i] = alpha * (expf(ptr[i]) - 1.f); // ncnn里面ELU实现，实际上就是对输入进行了一个激活函数的变换
        }
    }

    return 0;
}

} // namespace ncnn

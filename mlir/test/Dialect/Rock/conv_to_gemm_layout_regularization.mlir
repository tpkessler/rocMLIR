// RUN: rocmlir-gen --arch gfx908 -p -operation conv2d -fil_layout gkyxc -in_layout ngchw -out_layout hwgkn \
// RUN: | rocmlir-opt --rock-affix-params --rock-conv-to-gemm \
// RUN: | FileCheck %s --check-prefix=FWD

// FWD: #rock.transform_map<affine_map<(d0, d1, d2, d3, d4) -> (d0, d1, d3, d4, d2)> by
// FWD-SAME: <PassThrough ["g", "k", "y", "x", "c"] at [0, 1, 3, 4, 2] -> ["g", "k", "y", "x", "c"] at [0, 1, 2, 3, 4]>
// FWD-NEXT: #rock.transform_map<affine_map<(d0, d1, d2, d3, d4) -> (d1, d4, d2, d3, d0)>
// FWD-SAME: <PassThrough ["ho", "wo", "go", "ko", "no"] at [1, 4, 2, 3, 0] -> ["ho", "wo", "go", "ko", "no"] at [0, 1, 2, 3, 4]>
// FWD-NOT: Unfold

// RUN: rocmlir-gen --arch gfx908 -fil_layout=gkyxc -in_layout=ngchw -out_layout=ngkhw -batchsize=64 -in_channels=128 -in_h=28 -in_w=28 -out_channels=128 -fil_h=3 -fil_w=3 --dilation_h=1 --dilation_w=1 --conv_stride_h=1 --conv_stride_w=1 --padding_h=1 --padding_w=1 -t f32 --operation conv2d_bwd_weight \
// RUN: | rocmlir-opt --rock-affix-params --rock-conv-to-gemm \
// RUN: | FileCheck %s --check-prefix=WRW_ATOMIC
// WRW_ATOMIC: #rock.transform_map<affine_map<(d0, d1, d2, d3, d4) -> (d0, d1, d3, d4, d2)> by
// WRW_ATOMIC-SAME: <PassThrough ["g", "k", "y", "x", "c"] at [0, 1, 3, 4, 2] -> ["g", "k", "y", "x", "c"] at [0, 1, 2, 3, 4]>
// WRW_ATOMIC-NOT: Unfold
// WRW_ATOMIC: storeMethod = atomic_add

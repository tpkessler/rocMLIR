; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc -march=amdgcn -mcpu=gfx90a -verify-machineinstrs < %s | FileCheck -check-prefix=GCN %s

define amdgpu_kernel void @vectorLoadCombine(<4 x i8>* %in, i32* %out) {
; GCN-LABEL: vectorLoadCombine:
; GCN:       ; %bb.0: ; %entry
; GCN-NEXT:    s_load_dwordx4 s[0:3], s[0:1], 0x24
; GCN-NEXT:    s_waitcnt lgkmcnt(0)
; GCN-NEXT:    v_mov_b32_e32 v0, s0
; GCN-NEXT:    v_mov_b32_e32 v1, s1
; GCN-NEXT:    flat_load_dword v2, v[0:1]
; GCN-NEXT:    s_mov_b32 s0, 0x6050400
; GCN-NEXT:    v_mov_b32_e32 v0, s2
; GCN-NEXT:    v_mov_b32_e32 v1, s3
; GCN-NEXT:    s_waitcnt vmcnt(0) lgkmcnt(0)
; GCN-NEXT:    v_bfe_u32 v3, v2, 8, 8
; GCN-NEXT:    v_and_b32_e32 v4, 0xff0000, v2
; GCN-NEXT:    v_perm_b32 v3, v3, v2, s0
; GCN-NEXT:    v_and_b32_e32 v2, 0xff000000, v2
; GCN-NEXT:    v_or3_b32 v2, v3, v4, v2
; GCN-NEXT:    flat_store_dword v[0:1], v2
; GCN-NEXT:    s_endpgm
entry:
  %0 = load <4 x i8>, <4 x i8>* %in, align 4
  %1 = extractelement <4 x i8> %0, i32 0
  %2 = extractelement <4 x i8> %0, i32 1
  %3 = extractelement <4 x i8> %0, i32 2
  %4 = extractelement <4 x i8> %0, i32 3
  %zext0 = zext i8 %1 to i32
  %zext1 = zext i8 %2 to i32
  %shift1 = shl nuw nsw i32 %zext1, 8
  %insert1 = or i32 %shift1, %zext0
  %zext2 = zext i8 %3 to i32
  %shift2 = shl nuw nsw i32 %zext2, 16
  %insert2 = or i32 %insert1, %shift2
  %zext3 = zext i8 %4 to i32
  %shift3 = shl nuw i32 %zext3, 24
  %insert3 = or i32 %insert2, %shift3
  store i32 %insert3, i32* %out
  ret void
}

define amdgpu_kernel void @vectorLoadShuffle(<4 x i8>* %in, i32* %out) {
; GCN-LABEL: vectorLoadShuffle:
; GCN:       ; %bb.0: ; %entry
; GCN-NEXT:    s_load_dwordx4 s[0:3], s[0:1], 0x24
; GCN-NEXT:    s_waitcnt lgkmcnt(0)
; GCN-NEXT:    v_mov_b32_e32 v0, s0
; GCN-NEXT:    v_mov_b32_e32 v1, s1
; GCN-NEXT:    flat_load_dword v2, v[0:1]
; GCN-NEXT:    s_mov_b32 s0, 0x6050400
; GCN-NEXT:    v_mov_b32_e32 v0, s2
; GCN-NEXT:    v_mov_b32_e32 v1, s3
; GCN-NEXT:    s_waitcnt vmcnt(0) lgkmcnt(0)
; GCN-NEXT:    v_bfe_u32 v3, v2, 16, 8
; GCN-NEXT:    v_lshlrev_b32_e32 v4, 8, v2
; GCN-NEXT:    v_perm_b32 v3, v3, v2, s0
; GCN-NEXT:    v_and_b32_e32 v4, 0xff0000, v4
; GCN-NEXT:    v_and_b32_e32 v2, 0xff000000, v2
; GCN-NEXT:    v_or3_b32 v2, v3, v4, v2
; GCN-NEXT:    flat_store_dword v[0:1], v2
; GCN-NEXT:    s_endpgm
entry:
  %0 = load <4 x i8>, <4 x i8>* %in, align 4
  %1 = extractelement <4 x i8> %0, i32 0
  %2 = extractelement <4 x i8> %0, i32 1
  %3 = extractelement <4 x i8> %0, i32 2
  %4 = extractelement <4 x i8> %0, i32 3
  %zext0 = zext i8 %1 to i32
  %zext1 = zext i8 %3 to i32
  %shift1 = shl nuw nsw i32 %zext1, 8
  %insert1 = or i32 %shift1, %zext0
  %zext2 = zext i8 %2 to i32
  %shift2 = shl nuw nsw i32 %zext2, 16
  %insert2 = or i32 %insert1, %shift2
  %zext3 = zext i8 %4 to i32
  %shift3 = shl nuw i32 %zext3, 24
  %insert3 = or i32 %insert2, %shift3
  store i32 %insert3, i32* %out
  ret void
}

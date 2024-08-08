source_filename = "test"
target datalayout = "e-m:e-p:64:64-i64:64-f80:128-n8:16:32:64-S128"

@global_var_600ff8 = local_unnamed_addr global i64 0
@global_var_600e20 = local_unnamed_addr global i64 0
@global_var_400944 = constant [9 x i8] c"%d\0A%d\0A%d\00"
@global_var_600e10 = local_unnamed_addr global i64 4195648
@global_var_600e18 = local_unnamed_addr global i64 4195616
@global_var_601040 = local_unnamed_addr global i8 0

define i64 @f1(i32 %arg1) local_unnamed_addr {
dec_label_pc_400570:
  %0 = add i32 %arg1, 32, !insn.addr !0
  %1 = zext i32 %0 to i64, !insn.addr !1
  ret i64 %1, !insn.addr !2
}

define i64 @f2(i32 %arg1) local_unnamed_addr {
dec_label_pc_400590:
  %0 = add i32 %arg1, 32, !insn.addr !3
  %1 = zext i32 %0 to i64, !insn.addr !4
  ret i64 %1, !insn.addr !5
}

define i64 @f3(i32 %arg1) local_unnamed_addr {
dec_label_pc_4005c0:
  %0 = add i32 %arg1, 32, !insn.addr !6
  %1 = zext i32 %0 to i64, !insn.addr !7
  ret i64 %1, !insn.addr !8
}

define i64 @f4(i32 %arg1) local_unnamed_addr {
dec_label_pc_400600:
  %storemerge = add i32 %arg1, 32
  %0 = zext i32 %storemerge to i64, !insn.addr !9
  ret i64 %0, !insn.addr !10
}

define i64 @f5(i32 %arg1) local_unnamed_addr {
dec_label_pc_400660:
  %stack_var_-12.0 = add i32 %arg1, 32
  %0 = zext i32 %stack_var_-12.0 to i64, !insn.addr !11
  ret i64 %0, !insn.addr !12
}

define i64 @main(i64 %argc, i8** %argv) local_unnamed_addr {
dec_label_pc_400840:
  %0 = ptrtoint i8** %argv to i64
  %1 = add i64 %0, 8, !insn.addr !13
  %2 = inttoptr i64 %1 to i64*, !insn.addr !13
  %3 = load i64, i64* %2, align 8, !insn.addr !13
  %4 = inttoptr i64 %3 to i8*, !insn.addr !14
  %5 = call i32 @atoi(i8* %4), !insn.addr !14
  %6 = call i64 @f1(i32 %5), !insn.addr !15
  %7 = call i64 @f2(i32 %5), !insn.addr !16
  %8 = call i64 @f3(i32 %5), !insn.addr !17
  %9 = and i64 %6, 4294967295, !insn.addr !18
  %10 = and i64 %7, 4294967295, !insn.addr !19
  %11 = and i64 %8, 4294967295, !insn.addr !20
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @global_var_400944, i64 0, i64 0), i64 %9, i64 %10, i64 %11), !insn.addr !21
  %13 = call i64 @f4(i32 xor (i32 ptrtoint ([9 x i8]* @global_var_400944 to i32), i32 ptrtoint ([9 x i8]* @global_var_400944 to i32))), !insn.addr !22
  %14 = call i64 @f5(i32 xor (i32 xor (i32 ptrtoint ([9 x i8]* @global_var_400944 to i32), i32 ptrtoint ([9 x i8]* @global_var_400944 to i32)), i32 xor (i32 ptrtoint ([9 x i8]* @global_var_400944 to i32), i32 ptrtoint ([9 x i8]* @global_var_400944 to i32)))), !insn.addr !23
  ret i64 0, !insn.addr !24
}

declare i32 @printf(i8*, ...) local_unnamed_addr

declare i32 @atoi(i8*) local_unnamed_addr

!0 = !{i64 4195706}
!1 = !{i64 4195712}
!2 = !{i64 4195716}
!3 = !{i64 4195754}
!4 = !{i64 4195760}
!5 = !{i64 4195764}
!6 = !{i64 4195814}
!7 = !{i64 4195826}
!8 = !{i64 4195830}
!9 = !{i64 4195916}
!10 = !{i64 4195920}
!11 = !{i64 4196159}
!12 = !{i64 4196163}
!13 = !{i64 4196442}
!14 = !{i64 4196448}
!15 = !{i64 4196459}
!16 = !{i64 4196470}
!17 = !{i64 4196481}
!18 = !{i64 4196496}
!19 = !{i64 4196499}
!20 = !{i64 4196502}
!21 = !{i64 4196506}
!22 = !{i64 4196516}
!23 = !{i64 4196526}
!24 = !{i64 4196543}

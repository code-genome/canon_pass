; ModuleID = 'function_struct_test.bc'
source_filename = "function_struct_test.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.Person = type { [50 x i8], i32, %struct.Address }
%struct.Address = type { [50 x i8], [50 x i8], [50 x i8], [10 x i8] }

@.str = private unnamed_addr constant [10 x i8] c"Name: %s\0A\00", align 1
@.str.1 = private unnamed_addr constant [9 x i8] c"Age: %d\0A\00", align 1
@.str.3 = private unnamed_addr constant [12 x i8] c"Street: %s\0A\00", align 1
@.str.4 = private unnamed_addr constant [10 x i8] c"City: %s\0A\00", align 1
@.str.5 = private unnamed_addr constant [11 x i8] c"State: %s\0A\00", align 1
@.str.6 = private unnamed_addr constant [9 x i8] c"Zip: %s\0A\00", align 1
@__const.main.person1 = private unnamed_addr constant %struct.Person { [50 x i8] c"John Doe\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", i32 30, %struct.Address { [50 x i8] c"123 Main St\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", [50 x i8] c"Exampleville\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", [50 x i8] c"CA\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", [10 x i8] c"12345\00\00\00\00\00" } }, align 4
@.str.7 = private unnamed_addr constant [16 x i8] c"Enter new age: \00", align 1
@.str.8 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@str = private unnamed_addr constant [9 x i8] c"Address:\00", align 1

; Function Attrs: noinline nounwind uwtable
define dso_local void @printPerson(%struct.Person* byval align 8) local_unnamed_addr #0 {
  %2 = getelementptr inbounds %struct.Person, %struct.Person* %0, i64 0, i32 0, i64 0
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str, i64 0, i64 0), i8* nonnull %2)
  %4 = getelementptr inbounds %struct.Person, %struct.Person* %0, i64 0, i32 1
  %5 = load i32, i32* %4, align 4, !tbaa !2
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.1, i64 0, i64 0), i32 %5)
  %7 = call i32 @puts(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i64 0, i64 0))
  %8 = getelementptr inbounds %struct.Person, %struct.Person* %0, i64 0, i32 2, i32 0, i64 0
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str.3, i64 0, i64 0), i8* nonnull %8)
  %10 = getelementptr inbounds %struct.Person, %struct.Person* %0, i64 0, i32 2, i32 1, i64 0
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.4, i64 0, i64 0), i8* nonnull %10)
  %12 = getelementptr inbounds %struct.Person, %struct.Person* %0, i64 0, i32 2, i32 2, i64 0
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.5, i64 0, i64 0), i8* nonnull %12)
  %14 = getelementptr inbounds %struct.Person, %struct.Person* %0, i64 0, i32 2, i32 3, i64 0
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.6, i64 0, i64 0), i8* nonnull %14)
  ret void
}

; Function Attrs: nounwind
declare dso_local i32 @printf(i8* nocapture readonly, ...) local_unnamed_addr #1

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %1 = alloca %struct.Person, align 8
  %2 = getelementptr inbounds %struct.Person, %struct.Person* %1, i64 0, i32 0, i64 0
  call void @llvm.lifetime.start.p0i8(i64 216, i8* nonnull %2) #3
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* nonnull align 8 %2, i8* align 4 getelementptr inbounds (%struct.Person, %struct.Person* @__const.main.person1, i64 0, i32 0, i64 0), i64 216, i1 false)
  %3 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.7, i64 0, i64 0))
  %4 = getelementptr inbounds %struct.Person, %struct.Person* %1, i64 0, i32 1
  %5 = call i32 (i8*, ...) @__isoc99_scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.8, i64 0, i64 0), i32* nonnull %4)
  call void @printPerson(%struct.Person* byval nonnull align 8 %1)
  call void @llvm.lifetime.end.p0i8(i64 216, i8* nonnull %2) #3
  ret i32 0
}

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start.p0i8(i64, i8* nocapture) #2

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i1) #2

; Function Attrs: nounwind
declare dso_local i32 @__isoc99_scanf(i8* nocapture readonly, ...) local_unnamed_addr #1

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end.p0i8(i64, i8* nocapture) #2

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main2() local_unnamed_addr #0 {
  %1 = tail call i32 @main()
  ret i32 0
}

; Function Attrs: nounwind
declare i32 @puts(i8* nocapture readonly) local_unnamed_addr #3

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { argmemonly nounwind }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.1 (tags/RELEASE_801/final)"}
!2 = !{!3, !6, i64 52}
!3 = !{!"Person", !4, i64 0, !6, i64 52, !7, i64 56}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
!6 = !{!"int", !4, i64 0}
!7 = !{!"Address", !4, i64 0, !4, i64 50, !4, i64 100, !4, i64 150}

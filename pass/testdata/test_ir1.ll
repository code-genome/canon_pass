@g = global i32 123, align 4
@gp = global i32* @g, align 8
@vec = global [3 x i32] [i32 1, i32 2, i32 3], align 4
@intptr = common global i32* null, align 8

define i32 @f0(i32 %a) {
  %1 = load i32*, i32** @gp, align 8
  %2 = load i32, i32* %1, align 4
  %3 = add nsw i32 %2, %a
  ret i32 %3
}

define i32 @f1(i32) {
  %2 = load i32, i32* getelementptr inbounds ([3 x i32], [3 x i32]* @vec, i64 0, i64 0), align 4
  %3 = add nsw i32 %2, %0
  ret i32 %3
}
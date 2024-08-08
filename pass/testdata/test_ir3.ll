%type1 =  type {
    i32,
    i32,
    double
}
%type2 = type {
    i32,
    i32,
    %type1
}

define i32* @f0(i32 %a, %type2* %b) {
  %1 = getelementptr %type2, %type2* %b, i32 0, i32 1
  ret i32* %1
}
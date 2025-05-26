
; ModuleID = 'main_module'
declare i32 @printf(i8*, ...)
declare void @print_int(i32)
declare void @print_char(i8)
declare void @print_string(i8*)
declare void @print_float(float)

@.str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@.str_char = private unnamed_addr constant [4 x i8] c"%c\0A\00", align 1
@.str_float = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@.str_Hello = private unnamed_addr constant [6 x i8] c"Hello\00", align 1

define i32 @add(i32 %a, i32 %b) {
entry:
  %add = add i32 %a, %b
  ret i32 %add
}

define i32 @multiply(i32 %x, i32 %y) {
entry:
  %mul = mul i32 %x, %y
  ret i32 %mul
}

define i32 @subtract(i32 %x, i32 %y) {
entry:
  %sub = sub i32 %x, %y
  ret i32 %sub
}

define i32 @divide(i32 %x, i32 %y) {
entry:
  %check = icmp eq i32 %y, 0
  br i1 %check, label %divzero, label %divnonzero

divzero:
  ret i32 0

divnonzero:
  %div = sdiv i32 %x, %y
  ret i32 %div
}

define i32 @mod(i32 %x, i32 %y) {
entry:
  %mod = srem i32 %x, %y
  ret i32 %mod
}

define i32 @main() {
entry:
  %a = add i32 2, 3
  %tmp1 = add i32 4, 5
  %b = mul i32 %tmp1, 6
  %tmp2 = add i32 1, 2
  %c = add i32 %tmp2, 3

  ; Compound assignments
  %x0 = add i32 10, 5
  %x1 = sub i32 %x0, 3
  %x2 = mul i32 %x1, 2
  %x3 = sdiv i32 %x2, 4
  %x4 = srem i32 %x3, 3
  %x5 = shl i32 %x4, 1
  %x6 = ashr i32 %x5, 1
  %x7 = and i32 %x6, 6
  %x8 = or i32 %x7, 3
  %x = xor i32 %x8, 2

  ; Unary operations
  %y0 = add i32 0, 1
  %y1 = add i32 %y0, 1
  %y2 = sub i32 %y1, 1
  %y = sub i32 %y2, 1

  ; Arithmetic and logical
  %d = add i32 %a, %b
  %e = sub i32 %a, %b
  %f = mul i32 %a, %b
  %g = sdiv i32 %a, %b
  %h = srem i32 %a, %b

  %cmp1 = icmp sgt i32 %a, %b
  %cmp2 = icmp sgt i32 %b, 0
  %i_bool = and i1 %cmp1, %cmp2
  %i = zext i1 %i_bool to i32

  %cmp3 = icmp slt i32 %a, %b
  %cmp4 = icmp ne i32 %b, 0
  %j_bool = or i1 %cmp3, %cmp4
  %j = zext i1 %j_bool to i32

  %k = sub i32 0, %a
  %l = xor i1 true, true
  %l_int = zext i1 %l to i32
  %m = xor i32 %a, -1
  %n = and i32 %a, %b

  %shl = shl i32 %a, 2
  %shr = ashr i32 %b, 1

  %cmp5 = icmp sgt i32 %a, %b
  %t = select i1 %cmp5, i32 %a, i32 %b

  ; Array operations
  %arr0 = alloca [5 x i32]
  %arr0_ptr = getelementptr [5 x i32], [5 x i32]* %arr0, i32 0, i32 0
  store i32 10, i32* %arr0_ptr
  %arr1_ptr = getelementptr [5 x i32], [5 x i32]* %arr0, i32 0, i32 1
  store i32 20, i32* %arr1_ptr
  %arr0_val = load i32, i32* %arr0_ptr
  %arr1_val = load i32, i32* %arr1_ptr
  %arr2_val = add i32 %arr0_val, %arr1_val
  %arr2_ptr = getelementptr [5 x i32], [5 x i32]* %arr0, i32 0, i32 2
  store i32 %arr2_val, i32* %arr2_ptr

  %add_call = call i32 @add(i32 %arr0_val, i32 %arr1_val)
  %arr3_ptr = getelementptr [5 x i32], [5 x i32]* %arr0, i32 0, i32 3
  store i32 %add_call, i32* %arr3_ptr

  %mul_call = call i32 @multiply(i32 %arr0_val, i32 2)
  %arr4_ptr = getelementptr [5 x i32], [5 x i32]* %arr0, i32 0, i32 4
  store i32 %mul_call, i32* %arr4_ptr

  call void @print_int(i32 %arr2_val)
  call void @print_int(i32 %add_call)
  call void @print_int(i32 %mul_call)

  %sum = call i32 @add(i32 3, i32 4)
  %product = call i32 @multiply(i32 3, i32 4)
  %difference = call i32 @subtract(i32 10, i32 6)
  %quotient = call i32 @divide(i32 8, i32 2)
  %remainder = call i32 @mod(i32 8, i32 3)

  call void @print_int(i32 %sum)
  call void @print_int(i32 %product)
  call void @print_int(i32 %difference)
  call void @print_int(i32 %quotient)
  call void @print_int(i32 %remainder)

  ; If-Else
  %cond = icmp sgt i32 %a, %b
  br i1 %cond, label %if.then, label %if.else

if.then:
  call void @print_int(i32 %a)
  br label %if.end

if.else:
  call void @print_int(i32 %b)
  br label %if.end

if.end:
  br label %while.cond

; While loop
while.cond:
  %w = phi i32 [8, %if.end], [%w_dec, %while.body]
  %w_check = icmp sgt i32 %w, 0
  br i1 %w_check, label %while.body, label %while.end

while.body:
  call void @print_int(i32 %w)
  %w_dec = sub i32 %w, 2
  br label %while.cond

while.end:
  br label %for.outer.cond

; For loop 1: i from 0 to 4
for.outer.cond:
  %i1 = phi i32 [0, %while.end], [%i1_next, %for.outer.inc]
  %i1_cmp = icmp slt i32 %i1, 5
  br i1 %i1_cmp, label %for.outer.body, label %for.outer.end

for.outer.body:
  call void @print_int(i32 %i1)
  br label %for.outer.inc

for.outer.inc:
  %i1_next = add i32 %i1, 1
  br label %for.outer.cond

for.outer.end:
  br label %for.i.cond

; Nested for loop
for.i.cond:
  %i2 = phi i32 [0, %for.outer.end], [%i2_next, %for.i.inc]
  %i2_cmp = icmp slt i32 %i2, 3
  br i1 %i2_cmp, label %for.i.body, label %for.i.end

for.i.body:
  br label %for.j.cond

for.j.cond:
  %j2 = phi i32 [0, %for.i.body], [%j2_next, %for.j.inc]
  %j2_cmp = icmp slt i32 %j2, 2
  br i1 %j2_cmp, label %for.j.body, label %for.j.end

for.j.body:
  %ij_cmp = icmp eq i32 %i2, %j2
  br i1 %ij_cmp, label %print100, label %print200

print100:
  call void @print_int(i32 100)
  br label %for.j.inc

print200:
  call void @print_int(i32 200)
  br label %for.j.inc

for.j.inc:
  %j2_next = add i32 %j2, 1
  br label %for.j.cond

for.j.end:
  br label %for.i.inc

for.i.inc:
  %i2_next = add i32 %i2, 1
  br label %for.i.cond

for.i.end:
  ; Print char and string
  call void @print_char(i8 65)
  call void @print_string(i8* getelementptr ([6 x i8], [6 x i8]* @.str_Hello, i32 0, i32 0))

  ; Float operations
  %f1 = fadd float 0x40091EB860000000, 0.0 ; 3.14
  %f2 = fadd float 1.5, 0.0
  %f_sum = fadd float %f1, %f2
  %f_prod = fmul float %f1, %f2
  call void @print_float(float %f_sum)
  call void @print_float(float %f_prod)

  ; Casting
  %cast_int = fptosi float %f1 to i32
  %cast_float = sitofp i32 %a to float
  call void @print_int(i32 %cast_int)
  call void @print_float(float %cast_float)

  ret i32 %sum
}

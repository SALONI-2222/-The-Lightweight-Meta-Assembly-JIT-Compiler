declare i32 @printf(i8*, ...)
declare void @print_int(i32)
declare void @print_char(i8)
declare void @print_string(i8*)
declare void @print_float(float)

@.str = private unnamed_addr constant [4 x i8] c"2007135473\0A\00", align 1
@.str_char = private unnamed_addr constant [4 x i8] c"€\0A\00", align 1
@.str_float = private unnamed_addr constant [4 x i8] c"2709734513907834600000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000.000000\0A\00", align 1

define i32 @add(i32 %x, i32 %y) {
entry:
  %add = add i32 %x, %y
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

@arr = global [100 x i32] zeroinitializer
@arr = global [100 x i32] zeroinitializer
@arr = global [100 x i32] zeroinitializer
@arr = global [100 x i32] zeroinitializer
@arr = global [100 x i32] zeroinitializer
@arr = global [100 x i32] zeroinitializer
@arr = global [100 x i32] zeroinitializer
@arr = global [100 x i32] zeroinitializer
@arr = global [100 x i32] zeroinitializer
@arr = global [100 x i32] zeroinitializer
@arr = global [100 x i32] zeroinitializer
@arr = global [100 x i32] zeroinitializer
@arr = global [100 x i32] zeroinitializer
@.str0 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1

define i32 @main() {
entry:
add:
  ; Unhandled op 42
  ; Unhandled op 7
  ; Unhandled op 42
  ; Unhandled op 7
  ; Unhandled op 6
  ; Unhandled op 6
  v7 = add i32 %v2, %v3
  ret i32 %v4
multiply:
  ; Unhandled op 42
  ; Unhandled op 7
  ; Unhandled op 42
  ; Unhandled op 7
  ; Unhandled op 6
  ; Unhandled op 6
  v16 = mul i32 %v7, %v8
  ; Unhandled op 7
  ; Unhandled op 6
  ret i32 %v10
subtract:
  ; Unhandled op 42
  ; Unhandled op 7
  ; Unhandled op 42
  ; Unhandled op 7
  ; Unhandled op 6
  ; Unhandled op 6
  v27 = sub i32 %v13, %v14
  ret i32 %v15
divide:
  ; Unhandled op 42
  ; Unhandled op 7
  ; Unhandled op 42
  ; Unhandled op 7
  ; Unhandled op 6
  v35 = add i32 0, 0
  ; Unhandled op 20
  %cmp = icmp eq i32 %v20, 0
  br i1 %cmp, label %else_0, label %continue38
continue38:
  v39 = add i32 0, 0
  ret i32 %v21
  br label %endif_1
else_0:
endif_1:
  ; Unhandled op 6
  ; Unhandled op 6
  v46 = sdiv i32 %v22, %v23
  ret i32 %v24
mod:
  ; Unhandled op 42
  ; Unhandled op 7
  ; Unhandled op 42
  ; Unhandled op 7
  ; Unhandled op 6
  ; Unhandled op 6
  v55 = srem i32 %v27, %v28
  ret i32 %v29
main:
  v58 = add i32 0, 2
  v59 = add i32 0, 3
  v60 = add i32 0, 5
  ; Unhandled op 7
  v62 = add i32 0, 4
  v63 = add i32 0, 5
  v64 = add i32 0, 9
  v65 = add i32 0, 6
  v66 = add i32 0, 54
  ; Unhandled op 7
  v68 = add i32 0, 1
  v69 = add i32 0, 2
  v70 = add i32 0, 3
  v71 = add i32 0, 3
  v72 = add i32 0, 6
  ; Unhandled op 7
  v74 = add i32 0, 10
  ; Unhandled op 7
  ; Unhandled op 6
  v77 = add i32 0, 5
  v78 = add i32 %v44, %v45
  ; Unhandled op 7
  ; Unhandled op 6
  v81 = add i32 0, 3
  v82 = sub i32 %v47, %v48
  ; Unhandled op 7
  ; Unhandled op 6
  v85 = add i32 0, 2
  v86 = mul i32 %v50, %v51
  ; Unhandled op 7
  ; Unhandled op 6
  v89 = add i32 0, 4
  v90 = sdiv i32 %v53, %v54
  ; Unhandled op 7
  ; Unhandled op 6
  v93 = add i32 0, 3
  v94 = srem i32 %v56, %v57
  ; Unhandled op 7
  ; Unhandled op 6
  v97 = add i32 0, 1
  ; Unhandled op 31
  ; Unhandled op 7
  ; Unhandled op 6
  v101 = add i32 0, 1
  ; Unhandled op 32
  ; Unhandled op 7
  ; Unhandled op 6
  v105 = add i32 0, 6
  ; Unhandled op 28
  ; Unhandled op 7
  ; Unhandled op 6
  v109 = add i32 0, 3
  ; Unhandled op 29
  ; Unhandled op 7
  ; Unhandled op 6
  v113 = add i32 0, 2
  ; Unhandled op 30
  ; Unhandled op 7
  v116 = add i32 0, 0
  ; Unhandled op 7
  ; Unhandled op 6
  v119 = add i32 0, 1
  v120 = add i32 %v75, %v76
  ; Unhandled op 7
  ; Unhandled op 6
  v123 = add i32 0, 1
  v124 = add i32 %v78, %v79
  ; Unhandled op 7
  ; Unhandled op 6
  v127 = add i32 0, 1
  v128 = sub i32 %v81, %v82
  ; Unhandled op 7
  ; Unhandled op 6
  v131 = add i32 0, 1
  v132 = sub i32 %v84, %v85
  ; Unhandled op 7
  ; Unhandled op 6
  ; Unhandled op 6
  v136 = add i32 %v87, %v88
  ; Unhandled op 7
  ; Unhandled op 6
  ; Unhandled op 6
  v140 = sub i32 %v90, %v91
  ; Unhandled op 7
  ; Unhandled op 6
  ; Unhandled op 6
  v144 = mul i32 %v93, %v94
  ; Unhandled op 7
  ; Unhandled op 6
  ; Unhandled op 6
  v148 = sdiv i32 %v96, %v97
  ; Unhandled op 7
  ; Unhandled op 6
  ; Unhandled op 6
  v152 = srem i32 %v99, %v100
  ; Unhandled op 7
  ; Unhandled op 6
  ; Unhandled op 6
  ; Unhandled op 23
  ; Unhandled op 6
  v158 = add i32 0, 0
  ; Unhandled op 23
  ; Unhandled op 28
  ; Unhandled op 7
  ; Unhandled op 6
  ; Unhandled op 6
  ; Unhandled op 22
  ; Unhandled op 6
  v166 = add i32 0, 0
  ; Unhandled op 21
  ; Unhandled op 29
  ; Unhandled op 7
  ; Unhandled op 6
  ; Unhandled op 13
  ; Unhandled op 7
  v173 = add i32 0, 0
  ; Unhandled op 18
  ; Unhandled op 7
  ; Unhandled op 6
  ; Unhandled op 19
  ; Unhandled op 7
  ; Unhandled op 6
  ; Unhandled op 6
  ; Unhandled op 28
  ; Unhandled op 7
  ; Unhandled op 6
  v184 = add i32 0, 2
  ; Unhandled op 31
  ; Unhandled op 7
  ; Unhandled op 6
  v188 = add i32 0, 1
  ; Unhandled op 32
  ; Unhandled op 7
  ; Unhandled op 6
  ; Unhandled op 6
  ; Unhandled op 23
  %cmp = icmp eq i32 %v133, 0
  br i1 %cmp, label %tern_else_2, label %continue195
continue195:
  ; Unhandled op 6
  ; Unhandled op 40
  br label %tern_end_3
tern_else_2:
  ; Unhandled op 6
  ; Unhandled op 40
tern_end_3:
  ; Unhandled op 7
  v204 = add i32 0, 5
  v205 = add i32 0, 0
  v206 = add i32 0, 10
  %arr_ptr = getelementptr inbounds [100 x i32], [100 x i32]* @arr, i32 0, i32 %v138
  store i32 %v139, i32* %arr_ptr
  v208 = add i32 0, 1
  v209 = add i32 0, 20
  %arr_ptr = getelementptr inbounds [100 x i32], [100 x i32]* @arr, i32 0, i32 %v140
  store i32 %v141, i32* %arr_ptr
  v211 = add i32 0, 2
  v212 = add i32 0, 0
  %arr_ptr = getelementptr inbounds [100 x i32], [100 x i32]* @arr, i32 0, i32 %v143
  v213 = load i32, i32* %arr_ptr
  v214 = add i32 0, 1
  %arr_ptr = getelementptr inbounds [100 x i32], [100 x i32]* @arr, i32 0, i32 %v145
  v215 = load i32, i32* %arr_ptr
  v216 = add i32 %v144, %v146
  %arr_ptr = getelementptr inbounds [100 x i32], [100 x i32]* @arr, i32 0, i32 %v142
  store i32 %v147, i32* %arr_ptr
  v218 = add i32 0, 3
  v219 = add i32 0, 0
  %arr_ptr = getelementptr inbounds [100 x i32], [100 x i32]* @arr, i32 0, i32 %v149
  v220 = load i32, i32* %arr_ptr
  ; Unhandled op 41
  v222 = add i32 0, 1
  %arr_ptr = getelementptr inbounds [100 x i32], [100 x i32]* @arr, i32 0, i32 %v151
  v223 = load i32, i32* %arr_ptr
  ; Unhandled op 41
  v225 = call i32 @add()
  %arr_ptr = getelementptr inbounds [100 x i32], [100 x i32]* @arr, i32 0, i32 %v148
  store i32 %v153, i32* %arr_ptr
  v227 = add i32 0, 4
  v228 = add i32 0, 0
  %arr_ptr = getelementptr inbounds [100 x i32], [100 x i32]* @arr, i32 0, i32 %v155
  v229 = load i32, i32* %arr_ptr
  ; Unhandled op 41
  v231 = add i32 0, 2
  ; Unhandled op 41
  v233 = call i32 @multiply()
  %arr_ptr = getelementptr inbounds [100 x i32], [100 x i32]* @arr, i32 0, i32 %v154
  store i32 %v158, i32* %arr_ptr
  v235 = add i32 0, 2
  %arr_ptr = getelementptr inbounds [100 x i32], [100 x i32]* @arr, i32 0, i32 %v159
  v236 = load i32, i32* %arr_ptr
  call void @print_int(i32 %v160)
  v238 = add i32 0, 3
  %arr_ptr = getelementptr inbounds [100 x i32], [100 x i32]* @arr, i32 0, i32 %v161
  v239 = load i32, i32* %arr_ptr
  call void @print_int(i32 %v162)
  v241 = add i32 0, 4
  %arr_ptr = getelementptr inbounds [100 x i32], [100 x i32]* @arr, i32 0, i32 %v163
  v242 = load i32, i32* %arr_ptr
  call void @print_int(i32 %v164)
  v244 = add i32 0, 3
  ; Unhandled op 41
  v246 = add i32 0, 4
  ; Unhandled op 41
  v248 = call i32 @add()
  ; Unhandled op 7
  v250 = add i32 0, 3
  ; Unhandled op 41
  v252 = add i32 0, 4
  ; Unhandled op 41
  v254 = call i32 @multiply()
  ; Unhandled op 7
  v256 = add i32 0, 10
  ; Unhandled op 41
  v258 = add i32 0, 6
  ; Unhandled op 41
  v260 = call i32 @subtract()
  ; Unhandled op 7
  v262 = add i32 0, 8
  ; Unhandled op 41
  v264 = add i32 0, 2
  ; Unhandled op 41
  v266 = call i32 @divide()
  ; Unhandled op 7
  v268 = add i32 0, 8
  ; Unhandled op 41
  v270 = add i32 0, 3
  ; Unhandled op 41
  v272 = call i32 @mod()
  ; Unhandled op 7
  ; Unhandled op 6
  call void @print_int(i32 %v180)
  ; Unhandled op 6
  call void @print_int(i32 %v181)
  ; Unhandled op 6
  call void @print_int(i32 %v182)
  ; Unhandled op 6
  call void @print_int(i32 %v183)
  ; Unhandled op 6
  call void @print_int(i32 %v184)
  ; Unhandled op 6
  ; Unhandled op 6
  ; Unhandled op 23
  %cmp = icmp eq i32 %v187, 0
  br i1 %cmp, label %else_4, label %continue288
continue288:
  ; Unhandled op 6
  call void @print_int(i32 %v188)
  br label %endif_5
else_4:
  ; Unhandled op 6
  call void @print_int(i32 %v189)
endif_5:
  v296 = add i32 0, 8
  ; Unhandled op 7
while_start_6:
  ; Unhandled op 6
  v300 = add i32 0, 0
  ; Unhandled op 23
  %cmp = icmp eq i32 %v193, 0
  br i1 %cmp, label %while_end_7, label %continue303
continue303:
  ; Unhandled op 6
  call void @print_int(i32 %v194)
  ; Unhandled op 6
  v307 = add i32 0, 2
  v308 = sub i32 %v195, %v196
  ; Unhandled op 7
  br label %while_start_6
while_end_7:
  v312 = add i32 0, 0
  ; Unhandled op 7
for_start_8:
  ; Unhandled op 6
  v316 = add i32 0, 5
  ; Unhandled op 22
  %cmp = icmp eq i32 %v201, 0
  br i1 %cmp, label %for_end_9, label %continue319
continue319:
  ; Unhandled op 6
  call void @print_int(i32 %v202)
  ; Unhandled op 6
  v323 = add i32 0, 1
  v324 = add i32 %v203, %v204
  ; Unhandled op 7
  br label %for_start_8
for_end_9:
  v328 = add i32 0, 0
  ; Unhandled op 7
for_start_10:
  ; Unhandled op 6
  v332 = add i32 0, 3
  ; Unhandled op 22
  %cmp = icmp eq i32 %v209, 0
  br i1 %cmp, label %for_end_11, label %continue335
continue335:
  v336 = add i32 0, 0
  ; Unhandled op 7
for_start_12:
  ; Unhandled op 6
  v340 = add i32 0, 2
  ; Unhandled op 22
  %cmp = icmp eq i32 %v213, 0
  br i1 %cmp, label %for_end_13, label %continue343
continue343:
  ; Unhandled op 6
  ; Unhandled op 6
  ; Unhandled op 20
  %cmp = icmp eq i32 %v216, 0
  br i1 %cmp, label %else_14, label %continue348
continue348:
  v349 = add i32 0, 100
  call void @print_int(i32 %v217)
  br label %endif_15
else_14:
  v353 = add i32 0, 200
  call void @print_int(i32 %v218)
endif_15:
  ; Unhandled op 6
  v357 = add i32 0, 1
  v358 = add i32 %v219, %v220
  ; Unhandled op 7
  br label %for_start_12
for_end_13:
  ; Unhandled op 6
  v363 = add i32 0, 1
  v364 = add i32 %v222, %v223
  ; Unhandled op 7
  br label %for_start_10
for_end_11:
  v368 = add i32 0, 65
  ; Unhandled op 7
  ; Unhandled op 6
  call void @print_int(i32 %v226)
  ; Unhandled op 14
  ; Unhandled op 7
  ; Unhandled op 6
  call void @print_int(i32 %v228)
  ; Unhandled op 0
  ; Unhandled op 7
  ; Unhandled op 0
  ; Unhandled op 7
  ; Unhandled op 6
  ; Unhandled op 6
  v382 = add i32 %v231, %v232
  ; Unhandled op 7
  ; Unhandled op 6
  ; Unhandled op 6
  v386 = mul i32 %v234, %v235
  ; Unhandled op 7
  ; Unhandled op 6
  call void @print_int(i32 %v237)
  ; Unhandled op 6
  call void @print_int(i32 %v238)
  ; Unhandled op 6
  ; Unhandled op 17
  ; Unhandled op 7
  ; Unhandled op 6
  ; Unhandled op 16
  ; Unhandled op 7
  ; Unhandled op 6
  call void @print_int(i32 %v243)
  ; Unhandled op 6
  call void @print_int(i32 %v244)
  ; Unhandled op 6
  ret i32 %v245
  ret i32 0
}

# from mymodule import Mymodule

import mymodule

mm = mymodule.Mymodule()

print(mm.increment())
print(mm.increment())
print(mm.increment())

mm.obj = 987
print(mm.obj)

print(mm.str_of_obj_concat_str_arg('567'))
print(mm.str_of_obj_concat_str_arg('5678'))


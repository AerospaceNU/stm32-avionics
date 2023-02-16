"""
Edit this file to change the possible filter data struct values.
"""

STRUCT_ENTRIES = [
    ("double", "heading"),
    ("double", "vtg"),
    ("double", "pos_x"),
    ("double", "pos_y"),
    ("double", "pos_z_agl"),
    ("double", "rocket_vel_x"),
    ("double", "rocket_vel_y"),
    ("double", "rocket_vel_z"),
    ("double", "rocket_acc_x"),
    ("double", "rocket_acc_y"),
    ("double", "rocket_acc_z"),
    ("double", "world_vel_x"),
    ("double", "world_vel_y"),
    ("double", "world_vel_z"),
    ("double", "world_acc_x"),
    ("double", "world_acc_y"),
    ("double", "world_acc_z"),
    ("float", "rocket_ang_vel_x"),
    ("float", "rocket_ang_vel_y"),
    ("float", "rocket_ang_vel_z"),
    ("double", "qx"),
    ("double", "qy"),
    ("double", "qz"),
    ("double", "qw")
]

with open("filterdata_types.h", "w") as data_file:
    print("/**\n" +
          " * This file SHOULD NOT be edited manually.\n" +
          " * You should edit `filter_data_gen.py`\n" +
          " * and run it with Python to generate this file automatically.\n" +
          " */", file=data_file)
    print("#ifndef UTILS_FILTERDATA_TYPES_H_", file=data_file)
    print("#define UTILS_FILTERDATA_TYPES_H_", file=data_file)
    print("#ifdef __cplusplus\n"
          + 'extern "C" { \n'
          + "#endif\n"
          + "#include <stdbool.h>\n"
          + "#include <string.h>", file=data_file)
    print("typedef struct {", file=data_file)
    for entry in STRUCT_ENTRIES:
        print("  " + " ".join(entry) + ";", file=data_file)

    print("} FilterData_s;\n", file=data_file)

    print("typedef enum {", file=data_file)
    print(f"  invalid,", file=data_file)
    for entry in STRUCT_ENTRIES:
        print(f"  {entry[1]},", file=data_file)

    print("} FilterData_e;\n", file=data_file)

    print("FilterData_e getVariableEnumFromString(char* input);", file=data_file)
    print("float getVariableValue(FilterData_s *filterData, FilterData_e valueType);", file=data_file)
    print("void fillVariableName(char *buffer, int n, FilterData_e valueType);", file=data_file);
    print("#ifdef __cplusplus\n"
          + '}\n'
          + "#endif\n"
          + "#endif // UTILS_FILTERDATA_TYPES_H_", file=data_file)

with open("filterdata_types.c", "w") as data_file:
    print("/**\n" +
          " * This file SHOULD NOT be edited manually.\n" +
          " * You should edit `filter_data_gen.py`\n" +
          " * and run it with Python to generate this file automatically.\n" +
          " */", file=data_file)
    print('#include "filterdata_types.h"', file=data_file)
    print("FilterData_e getVariableEnumFromString(char* input) {", file=data_file)
    for entry in STRUCT_ENTRIES:
        print(f'  if (strncmp(input, "{entry[1]}", {len(entry[1])}) == 0) {{', file=data_file)
        print(f'    return {entry[1]};', file=data_file)
        print("  }", file=data_file)
    print("  // Matched no other valid value", file=data_file)
    print("  return invalid;", file=data_file)
    print("}", file=data_file)
    print("float getVariableValue(FilterData_s *filterData, FilterData_e valueType) {", file=data_file)
    print("  switch(valueType) {", file=data_file)
    print(f'    case invalid:', file=data_file)
    print(f'      return 0;', file=data_file)
    for entry in STRUCT_ENTRIES:
        print(f'    case {entry[1]}:', file=data_file)
        print(f'      return filterData->{entry[1]};', file=data_file)

    print("    default:\n      return 0;", file=data_file)
    print("  }\n}", file=data_file)

    print("void fillVariableName(char *buffer, int n, FilterData_e valueType) {", file=data_file)
    print("  switch(valueType) {", file=data_file)
    print(f'    case invalid:', file=data_file)
    print(f'      strncpy(buffer, "invalid", n);', file=data_file)
    print(f'      return;', file=data_file)
    for entry in STRUCT_ENTRIES:
        print(f'    case {entry[1]}:', file=data_file)
        print(f'      strncpy(buffer,  "{entry[1]}", n);', file=data_file)
        print(f'      return;', file=data_file)
    print("  }", file=data_file)
    print("}", file=data_file)

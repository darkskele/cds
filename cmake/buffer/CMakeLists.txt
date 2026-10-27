# buffer. Fixed buffers written in place and lent out.
add_library(cds_buffer INTERFACE)
add_library(cds::buffer ALIAS cds_buffer)
target_include_directories(cds_buffer INTERFACE ${PROJECT_SOURCE_DIR}/include)
target_compile_features(cds_buffer INTERFACE cxx_std_26)

# queue. Bounded lock-free queues.
add_library(cds_queue INTERFACE)
add_library(cds::queue ALIAS cds_queue)
target_include_directories(cds_queue INTERFACE ${PROJECT_SOURCE_DIR}/include)
target_compile_features(cds_queue INTERFACE cxx_std_26)
target_link_libraries(cds_queue INTERFACE Threads::Threads)

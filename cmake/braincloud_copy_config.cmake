# Copies the setup tool's braincloud.cfg beside the app, once one has been written.
if (EXISTS "${SRC}")
    execute_process(COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${SRC}" "${DST}")
endif()

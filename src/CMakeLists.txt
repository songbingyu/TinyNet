# add by bingyu.song  for TinyNet 


aux_source_directory(./ SRC_LIST)
include_directories( ./ )

MESSAGE(STATUS "This is SOURCE dir "${TinyNet_SOURCE_DIR})


ADD_LIBRARY(TinyNet  STATIC ${SRC_LIST})




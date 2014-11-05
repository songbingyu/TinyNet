#add by bingyu for TinyNet 

AUX_SOURCE_DIRECTORY(./ SRC_LIST)
INCLUDE_DIRECTORIES( ./ )
INCLUDE_DIRECTORIES( ../../src/  )

LINK_DIRECTORIES(../../lib )


ADD_EXECUTABLE( pp_client ${SRC_LIST})
TARGET_LINK_LIBRARIES( pp_client  TinyNet )
#TARGET_LINK_LIBRARIES( pp_client profiler )

#add by bingyu for TinyNet 

AUX_SOURCE_DIRECTORY(./ SRC_LIST)
INCLUDE_DIRECTORIES( ./ )
INCLUDE_DIRECTORIES( ../src/  )

LINK_DIRECTORIES(../lib )


ADD_EXECUTABLE( server_test ${SRC_LIST})
TARGET_LINK_LIBRARIES( server_test TinyNet )



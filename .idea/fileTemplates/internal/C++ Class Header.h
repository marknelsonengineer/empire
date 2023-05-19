#parse("C File Header.h")
#pragma once

#if (${HEADER_FILENAME})
#[[#include]]# "${HEADER_FILENAME}"
#end

${NAMESPACES_OPEN}

class ${NAME} {

};

${NAMESPACES_CLOSE}

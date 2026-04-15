#!/bin/sh

# Usage: ./cppclass98.sh ClassName

CLASS_NAME="$1"

if [ -z "$CLASS_NAME" ]; then
  echo "Usage: $0 ClassName"
  exit 1
fi

HEADER="${CLASS_NAME}.hpp"
SOURCE="${CLASS_NAME}.cpp"

GUARD=$(echo "${CLASS_NAME}_HPP" | tr 'a-z' 'A-Z')

# ---------- Header ----------
cat > "$HEADER" <<EOF
#ifndef ${GUARD}
#define ${GUARD}

class $CLASS_NAME
{
public:
    $CLASS_NAME();
    $CLASS_NAME(const $CLASS_NAME& other);
    ~$CLASS_NAME();

    $CLASS_NAME& operator=(const $CLASS_NAME& other);
};

#endif
EOF

# ---------- Source ----------
cat > "$SOURCE" <<EOF
#include "$HEADER"

$CLASS_NAME::$CLASS_NAME()
{
}

$CLASS_NAME::$CLASS_NAME(const $CLASS_NAME& other)
{
    (void)other;
}

$CLASS_NAME::~$CLASS_NAME()
{
}

$CLASS_NAME& $CLASS_NAME::operator=(const $CLASS_NAME& other)
{
    if (this != &other)
    {
        // copy members here
        (void)other;
    }
    return *this;
}
EOF

echo "Created $HEADER and $SOURCE (C++98 canonical form)"

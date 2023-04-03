def Settings( **kwargs ):
    return { 'flags': [
        '-x', 'c++', '-Wall', '-pedantic', '-std=c++17',
        '-I/usr/include',
        '-I./include',
        '-I./OpenGLWrapper/include',
        '-I./OpenGLWrapper/thirdparty/assimp/include',
        '-I./thirdparty/imgui',
    ],}

# MinecraftServerC

A Minecraft server built using the C programming language. The server should be capable of 1.20 features. The server should be built in a way that is both usable and scalable. The server should include Lua, Javascript, and Java modding support. The server should also allow for C/C++ libraries to be dynamically linked at runtime for even greater modding capabilities.

## Minimum Viable Product:

1. Basic server capabilities, described by https://wiki.vg/Protocol
2. Basic remote server management capabilities, described by https://developer.valvesoftware.com/wiki/Source_RCON_Protocol

## Additional content / features:

1. Custom modding support using:
    1. Java, preferably existing mods should be useable.
    2. Javascript, flexible yet easily useable.
    3. Lua, very basic but should work nonetheless.
2. Github like tools for backing up the server to a remote endpoint.
    1. The ability to commit the state of the server to a github like commit.
    2. The ability to push the current local commits to the remote endpoint.
    3. The ability to rollback the current local head to any given commit.
    4. The ability to branch the server off for any necessary debugging or creative testing for the end users.
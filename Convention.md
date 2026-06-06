# CONVENTIONS

### Naming Convention for Uniforms in the Shaders
`u_` means that the variable is a uniform

`a` before the name of a variable means that the variable is an attribute being set from the vertex array's addVertexBuffer function.
- `u_ModelMatrix`: `mat4` : model matrix
- `u_ViewProjection` : `mat4` : view projection matrix (_projection*_view)
- `u_TextureDiffuseX` : texture uniform : X is the slot number
- `u_TextureSpecularX` : texture uniform : X is the slot number

- `aPosition` : `vec3` : Position attribute
- `aNormal` : `vec3` : Normal attribute
- `aTexCoords` : `vec2` : Texture coordinates attribute

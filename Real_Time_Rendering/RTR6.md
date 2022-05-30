
# VI. Texturing
## Texturing Pipeline
- on the pixel level
  - instead of taking surface color, take from some image
  - the corresponding "pixel" of the pixel on that image (texture) is a **texel**
1. **texture mapping**: from object space to texture coordinates (parameter space) via a projection function
   - i.e. putting things into uv
2. *corresponders* map from paramter space to texture space, which can be used to retrieve texture information
3. the texture value is extracted from the texture map, using the texture-space coordinate
4. a *value transform function* processes the texture value and outputs it, where it will be used for some kind of modification

### Projector Function
- xyz to uv
- spherical, cylindrical, planar, natural projections
  ![](../res/real_time_rendering/171.1.png)
- usually actual projection functions are complex and dependent on the model/how the artist handles it in the modeling software
- 
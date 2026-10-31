### WORKSPACE:
- [ ] Implement AABB collision detection:
  AABB takes the maximum and minimum X and Y values, a total of 4 coordinates to check whether two objects are colliding.

### BACKLOG:
- [ ] Add Graphx goal, description and plans to [README.md](README.md)

### FINISHED:
- [x] Entity Component System:
  Objects and Models are replaced by Entities and Components. An entity refers to a number that its unique components can be identified by. A sparseset is used to keep the components dense for high cache locality in a dense vector yet identifiable by a sparse vector containing each components' index in the dense vector at it's entity index in the sparse vector. The sparse vector may have gaps to indicate that that specific entity does not own that type of component.
- [x] AssetManager, AssetRegistry, AssetPool:
  AssetRegistry registers all files in the asset/ folder and provides UUID->filepath lookups for them. AssetPool absorbs assets that are being used frequently and stores them for high cache locality. AssetManager orchestrates the AssetRegistry and AssetPool and provides a convenient high-level API.
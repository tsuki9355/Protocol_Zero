# Lays out the greybox arena in /Game/Maps/L_Arena_Greybox: perimeter walls, permanent cover,
# a navmesh bounds volume and 8 edge spawn points. Story M1-S07. Safe to re-run: it replaces
# everything it placed before. Run with the editor closed:
#   UnrealEditor.exe <abs>/Protocol_Zero.uproject -ExecutePythonScript="<abs>/tools/editor/make_arena_layout.py" -nosplash -unattended

import unreal

MAP = "/Game/Maps/L_Arena_Greybox"
PREFIXES = ("Wall_", "Cover_", "SpawnPoint_", "NavBounds")

HALF = 2000.0          # floor is 4000 x 4000 cm, centred on the origin
WALL_HEIGHT = 120.0    # stops movement without hiding the player from the isometric camera
COVER_HEIGHT = 200.0   # taller than the muzzle so cover blocks bolts
SPAWN_INSET = 150.0    # spawn points sit just inside the walls

les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
les.load_level(MAP)

for actor in eas.get_all_level_actors():
    if actor.get_actor_label().startswith(PREFIXES):
        eas.destroy_actor(actor)

cube = unreal.load_asset("/Engine/BasicShapes/Cube")


def block(label, x, y, size_x, size_y, height, folder):
    """Spawns a static cube with its base on the floor. Sizes are in cm; the source cube is 100 cm."""
    actor = eas.spawn_actor_from_object(cube, unreal.Vector(x, y, height / 2.0))
    actor.set_actor_label(label)
    actor.set_actor_scale3d(unreal.Vector(size_x / 100.0, size_y / 100.0, height / 100.0))
    actor.set_folder_path(folder)
    actor.static_mesh_component.set_mobility(unreal.ComponentMobility.STATIC)
    return actor


# Perimeter walls
span = HALF * 2.0 + 100.0
block("Wall_North", HALF + 50.0, 0.0, 100.0, span, WALL_HEIGHT, "Arena/Walls")
block("Wall_South", -HALF - 50.0, 0.0, 100.0, span, WALL_HEIGHT, "Arena/Walls")
block("Wall_East", 0.0, HALF + 50.0, span, 100.0, WALL_HEIGHT, "Arena/Walls")
block("Wall_West", 0.0, -HALF - 50.0, span, 100.0, WALL_HEIGHT, "Arena/Walls")

# Permanent cover: an inner ring of four blocks around the start, and four outer bars
for i, (x, y) in enumerate([(600, 600), (600, -600), (-600, 600), (-600, -600)]):
    block(f"Cover_Inner_{i}", x, y, 250.0, 250.0, COVER_HEIGHT, "Arena/Cover")
block("Cover_Outer_N", 1150.0, 0.0, 150.0, 600.0, COVER_HEIGHT, "Arena/Cover")
block("Cover_Outer_S", -1150.0, 0.0, 150.0, 600.0, COVER_HEIGHT, "Arena/Cover")
block("Cover_Outer_E", 0.0, 1150.0, 600.0, 150.0, COVER_HEIGHT, "Arena/Cover")
block("Cover_Outer_W", 0.0, -1150.0, 600.0, 150.0, COVER_HEIGHT, "Arena/Cover")

# Navmesh bounds covering the whole floor (the default volume brush is 200 cm)
nav = eas.spawn_actor_from_class(unreal.NavMeshBoundsVolume, unreal.Vector(0.0, 0.0, 100.0))
nav.set_actor_label("NavBounds")
nav.set_actor_scale3d(unreal.Vector(HALF / 100.0 + 1.0, HALF / 100.0 + 1.0, 3.0))
nav.set_folder_path("Arena")

# Edge spawn points, each facing the arena centre
edge = HALF - SPAWN_INSET
points = [
    (edge, 0.0), (-edge, 0.0), (0.0, edge), (0.0, -edge),
    (edge, edge), (edge, -edge), (-edge, edge), (-edge, -edge),
]
for i, (x, y) in enumerate(points):
    yaw = unreal.MathLibrary.find_look_at_rotation(unreal.Vector(x, y, 0.0), unreal.Vector(0.0, 0.0, 0.0)).yaw
    point = eas.spawn_actor_from_class(unreal.PZSpawnPoint, unreal.Vector(x, y, 100.0), unreal.Rotator(0.0, 0.0, yaw))
    point.set_actor_label(f"SpawnPoint_{i}")
    point.set_folder_path("Arena/SpawnPoints")

les.save_current_level()
counts = {p: sum(1 for a in eas.get_all_level_actors() if a.get_actor_label().startswith(p)) for p in PREFIXES}
unreal.log(f"PZ: arena layout saved {counts}")
unreal.SystemLibrary.quit_editor()

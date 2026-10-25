# Builds /Game/Maps/L_Arena_Greybox (floor, lighting, PlayerStart). Story M1-S01.
# Run with the editor closed:
#   UnrealEditor.exe <abs>/Protocol_Zero.uproject -ExecutePythonScript="<abs>/tools/editor/make_arena.py" -nosplash -unattended

import unreal

MAP = "/Game/Maps/L_Arena_Greybox"
les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

if unreal.EditorAssetLibrary.does_asset_exist(MAP):
    les.load_level(MAP)
else:
    les.new_level(MAP)

# Clear anything a previous run left behind
for a in eas.get_all_level_actors():
    if a.get_actor_label() in ("Floor", "Sun", "SkyLight", "SkyAtmosphere", "PlayerStart"):
        eas.destroy_actor(a)

plane = unreal.load_asset("/Engine/BasicShapes/Plane")
floor = eas.spawn_actor_from_object(plane, unreal.Vector(0, 0, 0))
floor.set_actor_label("Floor")
floor.set_actor_scale3d(unreal.Vector(40, 40, 1))

sun = eas.spawn_actor_from_class(unreal.DirectionalLight, unreal.Vector(0, 0, 1000), unreal.Rotator(0, -50, -30))
sun.set_actor_label("Sun")
sky = eas.spawn_actor_from_class(unreal.SkyLight, unreal.Vector(0, 0, 1200))
sky.set_actor_label("SkyLight")
sky.light_component.set_editor_property("real_time_capture", True)
eas.spawn_actor_from_class(unreal.SkyAtmosphere, unreal.Vector(0, 0, 0)).set_actor_label("SkyAtmosphere")

start = eas.spawn_actor_from_class(unreal.PlayerStart, unreal.Vector(0, 0, 100))
start.set_actor_label("PlayerStart")

les.save_current_level()
unreal.log("PZ: L_Arena_Greybox created")
unreal.SystemLibrary.quit_editor()

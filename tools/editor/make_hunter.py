# Creates /Game/Data/Enemies/DA_Hunter with placeholder tuning and places three test Hunters
# at edge spawn points in L_Arena_Greybox, until the wave system spawns them (M2). Story M1-S08.
# Safe to re-run. Run with the editor closed:
#   UnrealEditor.exe <abs>/Protocol_Zero.uproject -ExecutePythonScript="<abs>/tools/editor/make_hunter.py" -nosplash -unattended

import unreal

FOLDER = "/Game/Data/Enemies"
NAME = "DA_Hunter"
PATH = f"{FOLDER}/{NAME}"
MAP = "/Game/Maps/L_Arena_Greybox"
TEST_SPAWNS = ("SpawnPoint_0", "SpawnPoint_2", "SpawnPoint_7")

if unreal.EditorAssetLibrary.does_asset_exist(PATH):
    data = unreal.load_asset(PATH)
else:
    factory = unreal.DataAssetFactory()
    factory.set_editor_property("data_asset_class", unreal.PZDroneData)
    data = unreal.AssetToolsHelpers.get_asset_tools().create_asset(NAME, FOLDER, unreal.PZDroneData, factory)

# Placeholders until the tuning pass: fast, fragile, short-range melee on a cooldown.
data.set_editor_property("max_health", 30.0)
data.set_editor_property("move_speed", 550.0)
data.set_editor_property("attack_range", 150.0)
data.set_editor_property("attack_damage", 10.0)
data.set_editor_property("attack_cooldown", 1.0)
data.set_editor_property("destruction_duration", 0.4)  # M1-S09
data.set_editor_property("destruction_spin_rate", 900.0)
unreal.EditorAssetLibrary.save_loaded_asset(data)

les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
les.load_level(MAP)

actors = eas.get_all_level_actors()
for actor in actors:
    if actor.get_actor_label().startswith("TestHunter_"):
        eas.destroy_actor(actor)

spawns = {a.get_actor_label(): a for a in eas.get_all_level_actors() if a.get_actor_label() in TEST_SPAWNS}
for i, label in enumerate(TEST_SPAWNS):
    point = spawns[label]
    drone = eas.spawn_actor_from_class(unreal.PZDrone, point.get_actor_location(), point.get_actor_rotation())
    drone.set_actor_label(f"TestHunter_{i}")
    drone.set_folder_path("Arena/TestEnemies")

les.save_current_level()
placed = sum(1 for a in eas.get_all_level_actors() if a.get_actor_label().startswith("TestHunter_"))
unreal.log(f"PZ: {PATH} saved; {placed} test Hunters placed")
unreal.SystemLibrary.quit_editor()

# Creates the wave Data Assets in /Game/Data/Waves and removes the hand-placed test Hunters
# from L_Arena_Greybox now that waves spawn drones. Story M2-S03 (DA_Wave_01 only; the full
# roster lands in M2-S05). Safe to re-run. Run with the editor closed:
#   UnrealEditor.exe <abs>/Protocol_Zero.uproject -ExecutePythonScript="<abs>/tools/editor/make_waves.py" -nosplash -unattended

import unreal

FOLDER = "/Game/Data/Waves"
MAP = "/Game/Maps/L_Arena_Greybox"


def group(drone_class, count, delay):
    g = unreal.PZSpawnGroup()
    g.set_editor_property("drone_class", drone_class)
    g.set_editor_property("count", count)
    g.set_editor_property("delay_after_previous", delay)
    return g


# Placeholder until the M2-S05 roster: three small Hunter groups, then two Sentries (M2-S04).
WAVES = {
    "DA_Wave_01": ([group(unreal.PZDrone, 2, 0.0), group(unreal.PZDrone, 2, 5.0), group(unreal.PZDrone, 3, 5.0),
                    group(unreal.PZSentryDrone, 2, 5.0)], 1.0, 1.0),
}

tools = unreal.AssetToolsHelpers.get_asset_tools()
for name, (groups, health_mult, damage_mult) in WAVES.items():
    path = f"{FOLDER}/{name}"
    if unreal.EditorAssetLibrary.does_asset_exist(path):
        wave = unreal.load_asset(path)
    else:
        factory = unreal.DataAssetFactory()
        factory.set_editor_property("data_asset_class", unreal.PZWaveData)
        wave = tools.create_asset(name, FOLDER, unreal.PZWaveData, factory)
    wave.set_editor_property("spawn_groups", groups)
    wave.set_editor_property("health_multiplier", health_mult)
    wave.set_editor_property("damage_multiplier", damage_mult)
    unreal.EditorAssetLibrary.save_loaded_asset(wave, only_if_is_dirty=False)
    unreal.log(f"PZ: {path} saved with {wave.get_total_drone_count()} drones")

les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
les.load_level(MAP)
removed = 0
for actor in eas.get_all_level_actors():
    if actor.get_actor_label().startswith("TestHunter_"):
        eas.destroy_actor(actor)
        removed += 1
if removed:
    les.save_current_level()
unreal.log(f"PZ: removed {removed} test Hunters from {MAP}")
unreal.SystemLibrary.quit_editor()

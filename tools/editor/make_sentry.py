# Creates /Game/Data/Enemies/DA_Sentry with placeholder tuning. Story M2-S04.
# Run with the editor closed:
#   UnrealEditor-Cmd.exe <abs>/Protocol_Zero.uproject -run=pythonscript -script="<abs>/tools/editor/make_sentry.py"

import unreal

FOLDER = "/Game/Data/Enemies"
NAME = "DA_Sentry"
PATH = f"{FOLDER}/{NAME}"

if unreal.EditorAssetLibrary.does_asset_exist(PATH):
    data = unreal.load_asset(PATH)
else:
    factory = unreal.DataAssetFactory()
    factory.set_editor_property("data_asset_class", unreal.PZDroneData)
    data = unreal.AssetToolsHelpers.get_asset_tools().create_asset(NAME, FOLDER, unreal.PZDroneData, factory)

# Placeholders until the tuning pass: slower and sturdier than the Hunter, fires from range.
data.set_editor_property("max_health", 40.0)
data.set_editor_property("move_speed", 300.0)
data.set_editor_property("attack_range", 1400.0)
data.set_editor_property("attack_damage", 8.0)
data.set_editor_property("attack_cooldown", 1.5)
data.set_editor_property("preferred_range", 1000.0)
data.set_editor_property("resume_margin", 150.0)
data.set_editor_property("projectile_class", unreal.PZEnemyBolt)
data.set_editor_property("muzzle_offset", 60.0)
data.set_editor_property("destruction_duration", 0.4)
data.set_editor_property("destruction_spin_rate", 900.0)

unreal.EditorAssetLibrary.save_loaded_asset(data, only_if_is_dirty=False)
unreal.log(f"PZ: {PATH} saved")

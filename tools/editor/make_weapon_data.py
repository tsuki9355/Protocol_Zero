# Creates /Game/Data/Weapons/DA_PlasmaRifle with placeholder tuning. Story M1-S05.
# Run with the editor closed:
#   UnrealEditor-Cmd.exe <abs>/Protocol_Zero.uproject -run=pythonscript -script="<abs>/tools/editor/make_weapon_data.py"

import unreal

FOLDER = "/Game/Data/Weapons"
NAME = "DA_PlasmaRifle"
PATH = f"{FOLDER}/{NAME}"

if unreal.EditorAssetLibrary.does_asset_exist(PATH):
    asset = unreal.load_asset(PATH)
else:
    factory = unreal.DataAssetFactory()
    factory.set_editor_property("data_asset_class", unreal.PZWeaponData)
    asset = unreal.AssetToolsHelpers.get_asset_tools().create_asset(NAME, FOLDER, unreal.PZWeaponData, factory)

# Placeholders until the tuning pass: moderate damage, rapid fire.
asset.set_editor_property("damage", 10.0)
asset.set_editor_property("fire_cooldown", 0.1)

unreal.EditorAssetLibrary.save_loaded_asset(asset)
unreal.log(f"PZ: {PATH} saved (damage={asset.get_editor_property('damage')}, cooldown={asset.get_editor_property('fire_cooldown')})")

class CfgPatches
{
    class GSC_Vertical_Slot_Fix
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Data", "DZ_Scripts", "GSC_StalkerInventory_Scripts"};
    };
};

class CfgMods
{
    class GSC_Vertical_Slot_Fix
    {
        dir = "GSC_Vertical_Slot_Fix";
        name = "GSC Stalker Inventory - Vertical Slot Fix";
        type = "mod";
        dependencies[] = {"Mission"};
        class defs
        {
            class missionScriptModule { value = ""; files[] = {"GSC_Vertical_Slot_Fix/scripts/5_Mission"}; };
        };
    };
};

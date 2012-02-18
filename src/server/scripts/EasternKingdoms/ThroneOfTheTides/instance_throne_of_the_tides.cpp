#include "ScriptPCH.h"
#include "throne_of_the_tides.h"

enum Factions
{
    FACTION_FRIENDLY = 35
};

class instance_throne_of_the_tides : public InstanceMapScript
{
public:
    instance_throne_of_the_tides() : InstanceMapScript("instance_throne_of_the_tides", 643) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_throne_of_the_tides_InstanceMapScript(map);
    }

    struct instance_throne_of_the_tides_InstanceMapScript : public InstanceScript
    {
        instance_throne_of_the_tides_InstanceMapScript(Map* map) : InstanceScript(map)
        {
            SetBossNumber(MAX_ENCOUNTER);
        }

        uint64 uiLadyNazjar;
        uint64 uiCommanderUlthok;
        uint64 uiErunakStonespeaker;
        uint64 uiMindbenderGhursha;
        uint64 uiOzumat;
        uint64 uiNeptulon;

        uint64 uiCorales;
        uint64 uiLadyNazjarDoor;
        uint64 uiCommanderUlthokDoor;
        uint64 uiMindebenderGhurshaDoor;
        uint64 uiOzumatDoor;

        uint32 encounter[MAX_ENCOUNTER];

        std::string str_data;

        void Initialize()
        {
            for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                encounter[i] = NOT_STARTED;

            uiLadyNazjar = 0;
            uiCommanderUlthok = 0;
            uiErunakStonespeaker = 0;
            uiMindbenderGhursha = 0;
            uiOzumat = 0;
            uiNeptulon = 0;

            uiCorales = 0,
            uiLadyNazjarDoor = 0;
            uiCommanderUlthokDoor = 0;
            uiMindebenderGhurshaDoor = 0;
            uiOzumatDoor = 0;
        }

        bool IsEncounterInProgress() const
        {
            for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                if (encounter[i] == IN_PROGRESS) return true;

            return false;
        }

        void OnCreatureCreate(Creature* creature, bool /*add*/)
        {
            Map::PlayerList const &players = instance->GetPlayers();
            uint32 TeamInInstance = 0;

            if (!players.isEmpty())
            {
                if (Player* player = players.begin()->getSource())
                    TeamInInstance = player->GetTeam();
            }
            switch(creature->GetEntry())
            {
            case BOSS_LADY_NAZJAR:
                uiLadyNazjar = creature->GetGUID();
                break;
            case BOSS_COMMANDER_ULTHOK:
                if(encounter[0] == DONE)
                    creature->SetPhaseMask(PHASEMASK_NORMAL, true);
                uiCommanderUlthok = creature->GetGUID();
                break;
            case NPC_ERUNAK_STONESPEAKER:
                uiErunakStonespeaker = creature->GetGUID();
                break;
            case BOSS_MINDBENDER_GHURSHA:
                uiMindbenderGhursha = creature->GetGUID();
                break;
            case BOSS_OZUMAT:
                uiOzumat = creature->GetGUID();
                break;
            case NPC_NEPTULON:
                uiNeptulon = creature->GetGUID();
                break;
            case 50272:
                {
                    if (ServerAllowsTwoSideGroups())
                        creature->setFaction(FACTION_FRIENDLY);
                    if (TeamInInstance == ALLIANCE)
                        creature->UpdateEntry(50270, ALLIANCE);
                    break;
                }
            }
        }

        void OnGameObjectCreate(GameObject* go, bool /*add*/)
        {
            switch(go->GetEntry())
            {
            case GO_CORALES:
                uiCorales = go->GetGUID();
                if(encounter[0] == DONE)
                    go->SetPhaseMask(2,true);

                break;
            case GO_LADY_NAZJAR_DOOR:
                uiLadyNazjarDoor = go->GetGUID();
                HandleGameObject(uiLadyNazjarDoor, false);
                break;
            case GO_COMMANDER_ULTHOK_DOOR:
                uiCommanderUlthokDoor = go->GetGUID();
                HandleGameObject(0, (encounter[0] == DONE && encounter[1] == DONE) || encounter[0] == NOT_STARTED, go);
                break;
            case GO_ERUNAK_STONESPEAKER_DOOR:
                uiMindebenderGhurshaDoor = go->GetGUID();
                HandleGameObject(0, encounter[1] == DONE, go);
                break;
            case GO_OZUMAT_DOOR:
                uiOzumatDoor = go->GetGUID();
                HandleGameObject(0, encounter[2] == DONE, go);
                break;
            }
        }

        void SetData(uint32 type, uint32 data)
        {
            encounter[type] = data;

            switch(type)
            {
            case DATA_LADY_NAZJAR:
                if (data == DONE)
                {
                    if(Creature* commanderUlthok = instance->GetCreature(uiCommanderUlthok))
                        commanderUlthok->SetPhaseMask(PHASEMASK_NORMAL, true);

                    HandleGameObject(uiLadyNazjarDoor, true);

                    if(GameObject* corales = instance->GetGameObject(uiCorales))
                        corales->SetPhaseMask(2,true);

                }else if (data == IN_PROGRESS)
                {
                    HandleGameObject(uiLadyNazjarDoor, false);
                }else
                {
                    if(Creature* commanderUlthok = instance->GetCreature(uiCommanderUlthok))
                        commanderUlthok->SetPhaseMask(2, true);

                    HandleGameObject(uiLadyNazjarDoor, true);
                    HandleGameObject(uiCommanderUlthokDoor, false);

                    if(GameObject* corales = instance->GetGameObject(uiCorales))
                        corales->SetPhaseMask(1,true);
                }
                break;
            case DATA_COMMANDER_ULTHOK:
                if (data == DONE)
                {
                    HandleGameObject(uiCommanderUlthokDoor, true);
                    HandleGameObject(uiMindebenderGhurshaDoor, true);
                }else if (data == IN_PROGRESS)
                {
                break;
            case DATA_MINDEBENDER_GHURSHA:
                if(data == DONE)
                {
                    HandleGameObject(uiMindebenderGhurshaDoor, true);
                    HandleGameObject(uiOzumatDoor, true);
                }
                break;
            case DATA_OZUMAT:
                break;
            }

            if (data == DONE)
                SaveToDB();
        }

        uint32 GetData(uint32 type)
        {
            if(type < MAX_ENCOUNTER)
                return encounter[type];

            return NULL;
        }

        uint64 GetData64(uint32 identifier)
        {
            switch(identifier)
            {
            case BOSS_LADY_NAZJAR:
                return uiLadyNazjar;

            case BOSS_COMMANDER_ULTHOK:
                return uiCommanderUlthok;
            case NPC_ERUNAK_STONESPEAKER:
                return uiErunakStonespeaker;
            case BOSS_MINDBENDER_GHURSHA:
                return uiMindbenderGhursha;
            case BOSS_OZUMAT:
                return uiOzumat;
            case NPC_NEPTULON:
                return uiNeptulon;
            }

            return 0;
        }

        std::string GetSaveData()
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << "T o t T " << encounter[0] << " " << encounter[1] << " "
                << encounter[2] << " " << encounter[3];

            str_data = saveStream.str();

            OUT_SAVE_INST_DATA_COMPLETE;
            return str_data;
        }

        void Load(const char* in)
        {
            if (!in)
            {
                OUT_LOAD_INST_DATA_FAIL;
                return;
            }

            OUT_LOAD_INST_DATA(in);

            char dataHead1, dataHead2, dataHead3, dataHead4;
            uint16 data0, data1, data2, data3;

            std::istringstream loadStream(in);
            loadStream >> dataHead1 >> dataHead2 >> dataHead3 >> dataHead4 >> data0 >> data1 >> data2 >> data3;

            if (dataHead1 == 'T' && dataHead2 == 'o' && dataHead3 == 't' && dataHead4 == 'T')
            {
                encounter[0] = data0;
                encounter[1] = data1;
                encounter[2] = data2;
                encounter[3] = data3;

                for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                    if (encounter[i] == IN_PROGRESS)
                        encounter[i] = NOT_STARTED;

            } else OUT_LOAD_INST_DATA_FAIL;

            OUT_LOAD_INST_DATA_COMPLETE;
        }
    };
};

void AddSC_instance_throne_of_the_tides()
{
    new instance_throne_of_the_tides();
}
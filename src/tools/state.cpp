#include <string>

#include "global.h"
#include "state.h"

namespace Tools
{
    State::State() {
        m_History = new Entry();
        m_Current = m_History;
        m_Latest = m_History;

        m_CursorX = 0;
        m_CursorY = 0;

        m_UseDeg = 0;
    }
    State::~State() {
        delete m_History;
    }

    void State::Init() {
        std::string useDegSetting = globalconfig.GetValue("general.use_deg");
        if (useDegSetting.empty() || useDegSetting == "true")
            m_UseDeg = 1;
        else
            m_UseDeg = 0;
    }

    void State::CreateNewEntry() {
        Entry* newEntry = m_Latest->m_Next = new Entry();
        newEntry->m_Prev = m_Latest;
        m_Current = m_Latest = newEntry;
    }
}

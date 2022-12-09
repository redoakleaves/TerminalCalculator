#ifndef _ENTRY_H_
#define _ENTRY_H_

#include <stdint.h>
#include <string>

namespace Tools
{
    class Entry {
    public:
        uint32_t m_Identifier;

        std::string m_RawContent;

        Entry* m_Next;
        Entry* m_Prev;

        Entry();
        ~Entry();

        void SetStylized(std::string& stylized);
        std::string GetStylized();

        void SetResult(std::string& result);
        std::string GetResult();

        void PrintStylized();
        void PrintResult();

    private:
        std::string m_StylizedContent;
        std::string m_Result;
    };
}

#endif

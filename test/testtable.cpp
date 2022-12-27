/*
 * SPDX-FileCopyrightText: 2017-2017 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include "libime/table/tablebaseddictionary.h"
#include "libime/table/tabledecoder.h"
#include "libime/table/tableoptions.h"
#include "libime/table/tablerule.h"
#include "testdir.h"
#include <fcitx-utils/log.h>
#include <set>
#include <sstream>
#include <string>
#include <unistd.h>

using namespace libime;

void testMatch(const TableBasedDictionary &dict, std::string_view code,
               const std::set<std::string> &expect, bool exact) {
    std::set<std::string> actual;
    dict.matchWords(code,
                    exact ? TableMatchMode::Exact : TableMatchMode::Prefix,
                    [&actual](std::string_view, std::string_view word, uint32_t,
                              PhraseFlag) {
                        actual.insert(std::string{word});
                        return true;
                    });
    FCITX_ASSERT(expect == actual);
}

void testWubi() {

    std::string test = "KeyCode=abcdefghijklmnopqrstuvwxy\n"
                       "Length=4\n"
                       "Pinyin=@\n"
                       "[Rule]\n"
                       "e2=p11+p12+p21+p22\n"
                       "e3=p11+p21+p31+p32\n"
                       "a4=p11+p21+p31+n11\n"
                       "[Data]\n"
                       "xycq 统\n"
                       "yfh 计\n"
                       "nnkd 局\n";

    std::stringstream ss(test);

    try {
        libime::TableBasedDictionary table;
        table.load(ss, libime::TableFormat::Text);
        FCITX_ASSERT(table.hasRule());
        FCITX_ASSERT(table.hasPinyin());
        std::string key;
        FCITX_ASSERT(!table.generate("你好", key));
        FCITX_ASSERT(table.generate("统计局", key));
        FCITX_ASSERT(key == "xynn");

        FCITX_ASSERT(table.insert("wq", "你"));
        FCITX_ASSERT(table.insert("wqiy", "你"));
        FCITX_ASSERT(table.insert("v", "好"));
        FCITX_ASSERT(table.insert("vbg", "好"));

        table.save(LIBIME_BINARY_DIR "/test/testtable.dict");
        table.statistic();

        table.load(LIBIME_BINARY_DIR "/test/testtable.dict");
        table.statistic();
        // table.dump(std::cout);

        std::string key2;
        FCITX_ASSERT(table.generate("统计局", key2));
        FCITX_ASSERT(key == key2);
        FCITX_ASSERT(table.generate("你好", key2));
        FCITX_ASSERT(key2 == "wqvb") << key2;
        std::string key3;
        FCITX_ASSERT(table.generateWithHint("你好", {"abcd", "efgh"}, key3));
        FCITX_ASSERT(key3 == "abef");
        FCITX_ASSERT(table.generateWithHint("你好", {"abcd"}, key3));
        FCITX_ASSERT(key3 == "abvb");
        FCITX_ASSERT(table.generateWithHint("你好", {"", "abcd"}, key3));
        FCITX_ASSERT(key3 == "wqab");
        FCITX_ASSERT(table.generateWithHint("你好", {"", "a"}, key3));
        FCITX_ASSERT(key3 == "wqvb");
        FCITX_ASSERT(table.insert("你好"));
        testMatch(table, "wqvb", {"你好"}, false);
        testMatch(table, "wqvb", {"你好"}, true);
        testMatch(table, "w", {"你", "你好"}, false);
        testMatch(table, "wq", {"你", "你好"}, false);
        testMatch(table, "w", {}, true);
        table.insert("wo", "我", PhraseFlag::Pinyin);
        testMatch(table, "w", {"你", "你好", "我"}, false);

        FCITX_ASSERT(table.reverseLookup("你") == "wqiy");
        FCITX_ASSERT(table.reverseLookup("好") == "vbg");
        table.statistic();
        table.save(std::cout, TableFormat::Text);

        TableOptions options;
        options.setAutoRuleSet({"e2"});
        table.setTableOptions(options);

        graphForCode("wqvb", table)
            .dfs([](const SegmentGraphBase &graph,
                    const std::vector<size_t> &path) {
                size_t s = 0;
                for (auto e : path) {
                    std::cout << graph.segment(s, e) << " ";
                    s = e;
                }
                std::cout << std::endl;
                return true;
            });
        FCITX_ASSERT(table.wordExists("wqvb", "你好") == PhraseFlag::None);
        FCITX_ASSERT(table.wordExists("xyyf", "统计") == PhraseFlag::Invalid);
        FCITX_ASSERT(table.insert("统计", PhraseFlag::User));
        FCITX_ASSERT(table.wordExists("xyyf", "统计") == PhraseFlag::User);
    } catch (std::ios_base::failure &e) {
        std::cout << e.what() << std::endl;
    }
}

void testCangjie() {

    std::string test = "KeyCode=abcdefghijklmnopqrstuvwxyz\n"
                       "Length=6\n"
                       "Prompt=&\n"
                       "[Data]\n"
                       "&a 日\n"
                       "&b 月\n"
                       "&c 金\n"
                       "a 日\n"
                       "a 曰\n"
                       "aa 昌\n"
                       "aaa 晶\n"
                       "abac 暝\n";

    std::stringstream ss(test);

    try {
        libime::TableBasedDictionary table;
        table.load(ss, libime::TableFormat::Text);
        table.save(std::cout, TableFormat::Text);
        FCITX_ASSERT(!table.hasRule());
        std::string key;
        FCITX_ASSERT(!table.generate("你好", key));
        FCITX_ASSERT(table.hint("abac") == "日月日金");
    } catch (std::ios_base::failure &e) {
        std::cout << e.what() << std::endl;
    }
}

void testRule() {
    TableRule rule("e2=p11+p12+p21+p22+p00", 5);
    FCITX_ASSERT(rule.entries().size() == 5);
    bool ex = false;
    try {
        TableRule ruleBad("e2=p11+p12+p21+p22+p10", 5);
    } catch (...) {
        ex = true;
    }
    FCITX_ASSERT(ex);
    ex = false;
    try {
        TableRule ruleBad("e2=p11+p12+p21+p22+p01", 5);
    } catch (...) {
        ex = true;
    }
    FCITX_ASSERT(ex);
}

int main() {
    testRule();
    testWubi();
    testCangjie();

    return 0;
}

/*
 * SPDX-FileCopyrightText: 2017-2017 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */
#include "tableoptions.h"

namespace libime {

// mostly from table.desc, certain options are not related to libime
class TableOptionsPrivate {
public:
    OrderPolicy orderPolicy_ = OrderPolicy::No;
    uint32_t noSortInputLength_ = 0;
    uint32_t pinyinKey_ = '\0';
    bool autoSelect_ = false;
    int autoSelectLength_ = 0;
    std::string autoSelectRegex_;
    int noMatchAutoSelectLength_ = 0;
    std::string noMatchAutoSelectRegex_;
    bool commitRawInput_ = false;
    std::set<uint32_t> endKey_;
    uint32_t matchingKey_ = false;
    bool exactMatch_ = false;

    bool learning_ = true;
    int autoPhraseLength_ = -1;
    int saveAutoPhraseAfter_ = -1;
    std::unordered_set<std::string> autoRuleSet_;

    // show hint for word.
    bool prompt_ = false;
    // use prompt table
    bool displayCustomPromptSymbol_ = false;

    std::string languageCode_;

    bool sortByCodeLength_ = true;
};

TableOptions::TableOptions() : d_ptr(std::make_unique<TableOptionsPrivate>()) {}

FCITX_DEFINE_DPTR_COPY_AND_DEFAULT_DTOR_AND_MOVE(TableOptions)

FCITX_DEFINE_PROPERTY_PRIVATE(TableOptions, OrderPolicy, orderPolicy,
                              setOrderPolicy);
FCITX_DEFINE_PROPERTY_PRIVATE(TableOptions, uint32_t, noSortInputLength,
                              setNoSortInputLength);
FCITX_DEFINE_PROPERTY_PRIVATE(TableOptions, bool, autoSelect, setAutoSelect);
FCITX_DEFINE_PROPERTY_PRIVATE(TableOptions, int, autoSelectLength,
                              setAutoSelectLength);
FCITX_DEFINE_PROPERTY_PRIVATE(TableOptions, std::string, autoSelectRegex,
                              setAutoSelectRegex);
FCITX_DEFINE_PROPERTY_PRIVATE(TableOptions, int, noMatchAutoSelectLength,
                              setNoMatchAutoSelectLength);
FCITX_DEFINE_PROPERTY_PRIVATE(TableOptions, std::string, noMatchAutoSelectRegex,
                              setNoMatchAutoSelectRegex);
FCITX_DEFINE_PROPERTY_PRIVATE(TableOptions, bool, commitRawInput,
                              setCommitRawInput);
FCITX_DEFINE_PROPERTY_PRIVATE(TableOptions, std::set<uint32_t>, endKey,
                              setEndKey);
FCITX_DEFINE_PROPERTY_PRIVATE(TableOptions, uint32_t, matchingKey,
                              setMatchingKey);
FCITX_DEFINE_PROPERTY_PRIVATE(TableOptions, bool, exactMatch, setExactMatch);
FCITX_DEFINE_PROPERTY_PRIVATE(TableOptions, bool, learning, setLearning);
FCITX_DEFINE_PROPERTY_PRIVATE(TableOptions, int, autoPhraseLength,
                              setAutoPhraseLength);
FCITX_DEFINE_PROPERTY_PRIVATE(TableOptions, int, saveAutoPhraseAfter,
                              setSaveAutoPhraseAfter);
FCITX_DEFINE_PROPERTY_PRIVATE(TableOptions, std::string, languageCode,
                              setLanguageCode);
FCITX_DEFINE_PROPERTY_PRIVATE(TableOptions, std::unordered_set<std::string>,
                              autoRuleSet, setAutoRuleSet);
FCITX_DEFINE_PROPERTY_PRIVATE(TableOptions, bool, sortByCodeLength,
                              setSortByCodeLength);
} // namespace libime

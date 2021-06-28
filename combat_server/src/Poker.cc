#include "Poker.h"
#include "spdlog/spdlog.h"
#include <sstream>
const std::string pokerMap[] =
    {
        //黑桃
        "spadesA",
        "spades2",
        "spades3",
        "spades4",
        "spades5",
        "spades6",
        "spades7",
        "spades8",
        "spades9",
        "spades10",
        "spadesJ",
        "spadesQ",
        "spadesK",

        //红桃
        "heartsA",
        "hearts2",
        "hearts3",
        "hearts4",
        "hearts5",
        "hearts6",
        "hearts7",
        "hearts8",
        "hearts9",
        "hearts10",
        "heartsJ",
        "heartsQ",
        "heartsK",

        //梅花
        "clubsA",
        "clubs2",
        "clubs3",
        "clubs4",
        "clubs5",
        "clubs6",
        "clubs7",
        "clubs8",
        "clubs9",
        "clubs10",
        "clubsJ",
        "clubsQ",
        "clubsK",

        "diamondsA",
        "diamonds2",
        "diamonds3",
        "diamonds4",
        "diamonds5",
        "diamonds6",
        "diamonds7",
        "diamonds8",
        "diamonds9",
        "diamonds10",
        "diamondsJ",
        "diamondsQ",
        "diamondsK"

};
void Poker::showMessage() const
{
    std::stringstream ss;
    ss << "Poker"
       << "-isHide-" << this->_isHide << "-value-" << this->_value << "-poke-" << pokerMap[this->_value];
    spdlog::info(ss.str());
}
#include <iostream>
#include <thread>
#include <memory>

#include "viewer.hpp"
#include "logic.hpp"
#include "Perft.hpp"

int main()
{

    std::unique_ptr<Controller> controller = std::make_unique<Controller>();
    std::unique_ptr<Logic> logic = std::make_unique<Logic>(*controller);
    std::unique_ptr<Viewer> viewer = std::make_unique<Viewer>(*controller);

    std::thread viewerWorker(&Viewer::run, *viewer);

    viewerWorker.join();

    PerftTest(FEN_START_POS,5);

    //PerftTest(KIWIPETE, 1);
    //PerftTestMoveInfo(KIWIPETE);
    //PerftTestMoveInfo(FEN_START_POS);
    return 0;
}

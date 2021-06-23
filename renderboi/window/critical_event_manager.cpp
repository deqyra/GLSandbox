#include "critical_event_manager.hpp"

#include "gl_window.hpp"

namespace Renderboi
{

namespace Window
{

CriticalEventManager::CriticalEventManager(GLWindow* window) :
    _window(window),
    _criticalEventMutex(),
    _criticalEventQueue(),
    _criticalProcesses(),
    _processCount(0)
{

}

void CriticalEventManager::processCriticalEvents()
{
    _criticalEventMutex.lock();
    bool empty = _criticalEventQueue.empty();
    _criticalEventMutex.unlock();

    if (empty)
    {
        return;
    }

    // Request all critical processes to pause
    for (const auto &[id, process] : _criticalProcesses)
    {
        process->pause();
    }

    // Make sure all critical processes are paused
    for (const auto &[id, process] : _criticalProcesses)
    {
        process->waitUntilPaused(false);
    }

    // Process all awaiting events
    while (!_criticalEventQueue.empty())
    {
        _criticalEventMutex.lock();
        GLWindowCriticalEvent e = _criticalEventQueue.front();
        _criticalEventMutex.unlock();

        _processCriticalEvent(e);

        _criticalEventMutex.lock();
        _criticalEventQueue.pop();
        _criticalEventMutex.unlock();
    }

    // Resume all critical processes
    for (const auto &[id, process] : _criticalProcesses)
    {
        process->run();
    }
}

unsigned int CriticalEventManager::registerCriticalProcess(InterruptiblePtr process)
{
    if (process == nullptr)
    {
        throw std::runtime_error("CriticalEventManager: provided nullptr for process registration.");
    }

    _criticalProcesses[_processCount++] = process;

    return _processCount;
}

void CriticalEventManager::detachCriticalProcess(unsigned int registrationId)
{
    _criticalProcesses.erase(registrationId);
}

void CriticalEventManager::queueCriticalEvent(GLWindowCriticalEvent event)
{
    _criticalEventMutex.lock();
    _criticalEventQueue.push(event);
    _criticalEventMutex.unlock();
}

void CriticalEventManager::_processCriticalEvent(GLWindowCriticalEvent event)
{
    switch (event)
    {
        case GLWindowCriticalEvent::GoFullscreen:
            _window->goFullscreen(nullptr, true);
            break;

        case GLWindowCriticalEvent::ExitFullscreen:
            _window->exitFullscreen();
            break;
            
        default:
            std::string s = "CriticalEventManager: cannot process unknown "
            "critical event " + std::to_string(static_cast<int>(event)) + "."; 
            throw std::runtime_error(s.c_str());
    }
}

}// namespace Window

}// namespace Renderboi

@echo off
SLG.exe -D renderengine.type 4 -D screen.refresh.interval 100 -D path.sampler.type INLINED_RANDOM scenes\simple\render-fast.cfg
pause

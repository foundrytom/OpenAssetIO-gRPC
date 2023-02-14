let SessionLoad = 1
let s:so_save = &g:so | let s:siso_save = &g:siso | setg so=0 siso=0 | setl so=-1 siso=-1
let v:this_session=expand("<sfile>:p")
silent only
silent tabonly
cd ~/data/dev/OpenAssetIO-gRPC
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
let s:shortmess_save = &shortmess
if &shortmess =~ 'A'
  set shortmess=aoOA
else
  set shortmess=aoO
endif
badd +21 ~/data/dev/OpenAssetIO-gRPC/src/openassetio-grpc/include/openassetio-grpc/GRPCManagerImplementationFactory.hpp
badd +19 ~/data/dev/OpenAssetIO-gRPC/src/server/main.cpp
badd +8 ~/data/dev/OpenAssetIO-gRPC/src/server/CMakeLists.txt
badd +57 ~/data/dev/OpenAssetIO-gRPC/src/openassetio-grpc/CMakeLists.txt
badd +81 ~/data/dev/OpenAssetIO-gRPC/src/openassetio-grpc/src/GRPCManagerImplementationFactory.cpp
badd +22 ~/data/dev/OpenAssetIO-gRPC/src/CMakeLists.txt
badd +7 ~/.config/kitty/kitty.conf
badd +1 ~/.config/kitty/current-theme.conf
badd +2 cmake/CompilerWarnings.cmake
badd +16 ~/data/dev/OpenAssetIO-gRPC/CMakeLists.txt
badd +115 ~/data/dev/OpenAssetIO-gRPC/src/openassetio.proto
badd +52 ~/data/dev/OpenAssetIO-gRPC/src/openassetio-grpc/include/openassetio-grpc/GRPCManagerInterface.hpp
badd +39 ~/data/dev/OpenAssetIO-gRPC/cmake/DefaultTargetProperties.cmake
badd +17 ~/data/dev/OpenAssetIO-gRPC/src/test/CMakeLists.txt
badd +72 ~/data/dev/OpenAssetIO-gRPC/src/test/main.cpp
badd +56 ~/data/dev/external/grpc/grpc/examples/cpp/helloworld/greeter_client.cc
badd +49 ~/data/dev/OpenAssetIO/src/openassetio-core/include/openassetio/hostApi/ManagerImplementationFactoryInterface.hpp
badd +142 src/perf/main.cpp
badd +28 ~/data/dev/OpenAssetIO-gRPC/src/perf/CMakeLists.txt
badd +153 ~/data/dev/OpenAssetIO-gRPC/src/openassetio-grpc/src/GRPCManagerInterface.cpp
badd +8 ~/data/dev/OpenAssetIO-gRPC/conanfile.txt
badd +0 openassetio_conf.toml
badd +176 ~/data/dev/OpenAssetIO-gRPC/README.md
argglobal
%argdel
edit ~/data/dev/OpenAssetIO-gRPC/README.md
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
let &splitbelow = s:save_splitbelow
let &splitright = s:save_splitright
wincmd t
let s:save_winminheight = &winminheight
let s:save_winminwidth = &winminwidth
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
wincmd =
argglobal
balt cmake/CompilerWarnings.cmake
setlocal fdm=expr
setlocal fde=nvim_treesitter#foldexpr()
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=9999
setlocal fml=1
setlocal fdn=20
setlocal fen
let s:l = 176 - ((36 * winheight(0) + 36) / 72)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 176
normal! 05|
wincmd w
argglobal
if bufexists(fnamemodify("openassetio_conf.toml", ":p")) | buffer openassetio_conf.toml | else | edit openassetio_conf.toml | endif
if &buftype ==# 'terminal'
  silent file openassetio_conf.toml
endif
balt ~/data/dev/OpenAssetIO-gRPC/src/server/CMakeLists.txt
setlocal fdm=expr
setlocal fde=nvim_treesitter#foldexpr()
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=9999
setlocal fml=1
setlocal fdn=20
setlocal fen
let s:l = 2 - ((1 * winheight(0) + 36) / 72)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 2
normal! 015|
wincmd w
2wincmd w
wincmd =
tabnext 1
if exists('s:wipebuf') && len(win_findbuf(s:wipebuf)) == 0 && getbufvar(s:wipebuf, '&buftype') isnot# 'terminal'
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20
let &shortmess = s:shortmess_save
let &winminheight = s:save_winminheight
let &winminwidth = s:save_winminwidth
let s:sx = expand("<sfile>:p:r")."x.vim"
if filereadable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &g:so = s:so_save | let &g:siso = s:siso_save
let g:this_session = v:this_session
let g:this_obsession = v:this_session
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :

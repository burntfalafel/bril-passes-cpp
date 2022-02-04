nnoremap <silent> <leader>s :!tmux new-window -n "runner-vim" -d 'clang++ -Wall ConvertJsonToDot.cpp -ljsoncpp; sleep 15; exit' <CR>

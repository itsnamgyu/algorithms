# ho: personal notification script
tmux new-session -s 'algo' -d '
make analyze_all;
command -v ho &>/dev/null
if [[ $? == 0 ]]; then
	ho "algo analysis complete"
fi'

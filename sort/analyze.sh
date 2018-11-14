tmux new-session -s 'algo_analysis' -d 'make analyze_all | tee analysis_log'

# personal notification command
command -v ho &>/dev/null
if [[ $? == 0 ]]; then
	ho "algo analysis complete"
fi

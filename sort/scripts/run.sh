cd $(dirname $0)
cd ..

make analyze
./analyze

# personal notification
curl -X POST -H 'Content-type: application/json' --data "{\"text\":\"algo analysis complete\"}" https://hooks.slack.com/services/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

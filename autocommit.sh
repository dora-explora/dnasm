# autocommit to the github repo every 30 seconds.

cd /workspaces/dnasm

while true; do
    git add .
    git commit -m "autocommit"
    git push
    sleep 30
done
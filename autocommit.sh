# autocommit to the github repo every 5 minutes.

cd /workspaces/dnasm

while true; do
    git add .
    git commit -m "autocommit"
    git push
    sleep 300
done
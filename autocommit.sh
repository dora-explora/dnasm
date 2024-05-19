# autocommit to the github repo every minute

cd /workspaces/dnasm

while true; do
    git add .
    git commit -m "autocommit"
    git push
    sleep 60
done
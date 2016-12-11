echo -e "Original Author\n===============\nDamir Franusic <df@release14.org>" > AUTHORS
CONTRIBUTORS=`git log --format='%aN <%aE>' | sort -f | uniq | grep -v "Damir Franusic"`
if [ ! -z "$CONTRIBUTORS" ]; then
    echo -e "\nContributors\n============\n$CONTRIBUTORS" >> AUTHORS
fi

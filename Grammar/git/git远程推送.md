#
1. 本地仓库建立
2. init
3. git remote add origin https://gitee.com/blsm14/gile.git
4. git remote -v

curl -X POST --header 'Content-Type: application/json;charset=UTF-8' 'https://gitee.com/api/v5/user/repos' -d '{"access_token":"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx","name":"my-project","has_issues":"true","has_wiki":"true","can_comment":"true","private":"true"}'

curl --user blsm14:561759085274zd https://api.gitee.com/blsm14/projects -d '{"name":"pythonProject"}'

5. git push
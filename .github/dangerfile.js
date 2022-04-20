import { message, warn, markdown, danger } from 'danger'

// Check if branch name is correct
let branch_parts = process.env.BRANCH.split("/");
let pull_request_id = branch_parts.shift();
let allowed_branches = ["feature", "release", "hotfix", "refactor"];
if(!allowed_branches.includes(branch_parts[0]))
	fail(`Branch name is incorrect! \`${branch_parts[0]}\` is not a valid branch prefix! Pick one of ${allowed_branches.join(', ')}.`);

// Check if docs are updated
console.log(danger.git.modified_files, danger.git.created_files);

message(`Eyo ${process.env.BRANCH}`)

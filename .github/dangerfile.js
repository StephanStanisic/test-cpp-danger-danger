import { message, warn, markdown, danger } from 'danger'
import { default as LanguageDetect } from 'languagedetect'

// Check if branch name is correct
let branch_parts = process.env.BRANCH.split("/");
let pull_request_id = branch_parts.shift();
let allowed_branches = ["feature", "release", "hotfix", "refactor"];
if(!allowed_branches.includes(branch_parts[0]))
	fail(`Branch name is incorrect! \`${branch_parts[0]}\` is not a valid branch prefix! Pick one of ${allowed_branches.join(', ')}.`);

// Check if docs are updated
let modified_files = [...danger.git.modified_files, ...danger.git.created_files];
let modified_docs = modified_files.filter(file => file.startsWith("docs/"));
if(modified_docs.length == 0)
	warn("It looks like the docs aren't updated, are you sure you're not missing something?");

// Check if commit messages are english
let commit_messages = danger.git.commits.map(c => [c.message, c.sha.substr(5, 7)]);
let non_english_messages = commit_messages.filter(m => language_detector.detect(m[0])[0][0] !== "english");
non_english_messages.forEach(m => warn(`Commit \`${m[1]}\` might not be in english: '${m[0]}'`));

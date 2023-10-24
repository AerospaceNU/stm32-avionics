interface HttpRequestOptions {
    method?: string;
    url: string;
    referer?: string;
    resolveWithFullResponse?: boolean;
    form?: any;
    body?: any;
}
interface GraphQLRequestOptions {
    origin?: string;
    referer?: string;
    query: string;
    variables?: object;
}
interface Credit {
    session?: string;
    csrfToken: string;
}
declare enum ProblemStatus {
    "Accept" = 0,
    "Not Accept" = 1,
    "Not Start" = 2
}
declare enum ProblemDifficulty {
    "Easy" = 0,
    "Medium" = 1,
    "Hard" = 2
}
declare enum SubmissionStatus {
    "Accepted" = 0,
    "Compile Error" = 1,
    "Wrong Answer" = 2,
    "Time Limit Exceeded" = 3
}
declare enum EndPoint {
    "US" = 0,
    "CN" = 1
}
interface Uris {
    base: string;
    login: string;
    graphql: string;
    problemsAll: string;
    problem: string;
    submit: string;
    submission: string;
}
export { HttpRequestOptions, GraphQLRequestOptions, Credit, ProblemStatus, ProblemDifficulty, SubmissionStatus, EndPoint, Uris, };

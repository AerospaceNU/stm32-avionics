import { Credit, EndPoint, GraphQLRequestOptions, HttpRequestOptions, ProblemDifficulty, ProblemStatus, SubmissionStatus, Uris } from './interfaces';
declare class Helper {
    static credit: Credit;
    static uris: Uris;
    static setCredit(credit: Credit): void;
    static setUris(uris: Uris): void;
    static parseCookie(cookies: Array<string>, key: string): string;
    static levelToName(level: number): string;
    static statusMap(status: string | null): ProblemStatus;
    static difficultyMap(difficulty: number): ProblemDifficulty;
    static submissionStatusMap(submission: string): SubmissionStatus;
    static HttpRequest(options: HttpRequestOptions): Promise<any>;
    static GraphQLRequest(options: GraphQLRequestOptions): Promise<any>;
    static switchEndPoint(endPoint: EndPoint): void;
}
export default Helper;

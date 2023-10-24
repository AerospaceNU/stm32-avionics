import { SubmissionStatus, Uris } from "../utils/interfaces";
declare class Submission {
    id: number;
    isPending?: string | undefined;
    lang?: string | undefined;
    memory?: string | undefined;
    runtime?: string | undefined;
    status?: SubmissionStatus | undefined;
    timestamp?: number | undefined;
    code?: string | undefined;
    static uris: Uris;
    static setUris(uris: Uris): void;
    constructor(id: number, isPending?: string | undefined, lang?: string | undefined, memory?: string | undefined, runtime?: string | undefined, status?: SubmissionStatus | undefined, timestamp?: number | undefined, code?: string | undefined);
    detail(): Promise<Submission>;
}
export default Submission;


export interface AppPage {
    display: string,
    slug: string
}

export interface App {
    hasAuth: boolean,
    hasIdentified: boolean,
    hasInitialized: boolean,
    appPages: Array<AppPage>
};
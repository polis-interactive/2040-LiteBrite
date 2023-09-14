
export const AsyncTimeout = (ms: number): Promise<void> => {
    return new Promise((resolve) => {
        setTimeout(() => {
            resolve();
        }, ms)
    })
}

export const InternalNavigateTo: typeof navigateTo = (to) => {
    return navigateTo(`${to}?internal_guard=true`, { replace: true });
}
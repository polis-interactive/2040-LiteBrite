
export type ToastContext = 'success' | 'info' | 'warning' | 'error'

export class Toast {
    constructor(
        public message: string,
        public color: ToastContext,
        public timeout: number = 3000
    ) {}
    public id: number = -1;
    public position: number = -1;
    public isShow: boolean = true;
}
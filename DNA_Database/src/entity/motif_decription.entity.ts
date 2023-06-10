import { BaseEntity, Column, ColumnTypeUndefinedError, Entity, JoinColumn, JoinTable, ManyToMany, OneToOne, ManyToOne,OneToMany, PrimaryColumn, PrimaryGeneratedColumn } from "typeorm";
import { Interaction } from "./interaction.entity"

@Entity()
export class MotifDis {
    @PrimaryColumn()
    model: string

    @Column()
    quality: number

    @Column()
    entrez: number

    @Column()
    TF: number

    @Column()
    TFfamily: string

    @Column()
    Length:number

    @Column()
    UniportID: number


}